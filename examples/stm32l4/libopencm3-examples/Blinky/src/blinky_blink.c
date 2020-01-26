#ifndef STM32L4
   #define STM32L4
#endif


#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
 
 
 
 
static void delay_loop(int32_t loops)
{
    while(loops > 0)
    {
       asm("nop");
       loops--;
    }
}
 
int main(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO5);
    while(1)
    {
       gpio_toggle(GPIOA, GPIO5);
       delay_loop(400000);
    }
}