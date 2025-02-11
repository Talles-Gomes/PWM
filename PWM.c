#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h" //biblioteca para controlar o hardware de PWM

#define PWM_motor 22
#define PWM_led 12 // Definição do pino do LED

const uint16_t WRAP_PERIOD = 62500; //valor máximo do contador - WRAP
const float PWM_DIVISER = 40.0; //divisor do clock para o PWM
int carga=0;//variável de controle do estado do motor
uint16_t motor_level = 7500;// Torna o DC em 0,12% e põe o motor em 180º
const uint32_t motor_STEP = 16;//intervalo de mudança do motor
uint16_t led_level = 0; // Inicializa o nível do LED

int main() {
  stdio_init_all();

  gpio_set_function(PWM_motor, GPIO_FUNC_PWM); //habilitar o pino GPIO como PWM
  uint slice = pwm_gpio_to_slice_num(PWM_motor); //obter o canal PWM da GPIO
  pwm_set_clkdiv(slice, PWM_DIVISER); //define o divisor de clock do PWM
  pwm_set_wrap(slice, WRAP_PERIOD); //definir o valor de wrap
  pwm_set_enabled(slice, true); //habilita o pwm no slice correspondente

  gpio_set_function(PWM_led, GPIO_FUNC_PWM); //habilitar o pino do LED como PWM
  uint slice_led = pwm_gpio_to_slice_num(PWM_led); //obter o canal PWM da GPIO do LED
  pwm_set_clkdiv(slice_led, PWM_DIVISER); //define o divisor de clock do PWM
  pwm_set_wrap(slice_led, WRAP_PERIOD); //definir o valor de wrap
  pwm_set_enabled(slice_led, true); //habilita o pwm no slice correspondente

  uint up_down = 1;//variável de controle para alternar o motor em acréscimo e decréscimo
  while (true) {

    led_level=motor_level;

     pwm_set_gpio_level(PWM_motor, motor_level); //definir o ciclo de trabalho (duty cycle) do pwm
     pwm_set_gpio_level(PWM_led, led_level); //definir o ciclo de trabalho (duty cycle) do pwm do LED

    switch(carga)
    {
      case 0 :  
        carga++;
      break;
      case 1 :
        motor_level= 4594;// Torna o DC em 0,0735% e põe o motor em 90º
        sleep_ms(5000);
        carga++;
      break;
      case 2 :
        motor_level= 1563;// Torna o DC em 0,025% e põe o motor em 0º
        sleep_ms(5000);
        carga++;
      break;
      case 3 :
        sleep_ms(5000);//conclui a primeira etapa e joga pro loop de aumento e decremento do motor
        carga++;
      break;
      case 4 :
        sleep_ms(10);//delay de ajuste
        if(up_down ) 
        {
          motor_level += motor_STEP; // Incrementa o nível do motor
          if (motor_level >= 7500)
            up_down = 0; // Muda direção para diminuir quando atingir o período máximo
          }
          else
          { 
            motor_level -= motor_STEP; // Decrementa o nível do motor
          
          if (motor_level <= motor_STEP)
            up_down = 1; // Muda direção para aumentar quando atingir o mínimo
          }
      break;
    }
  }
}