#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "bsp/nano.h"

int main(void) {
    
    Timer0_Init();

    
    GPIO_Init(LED_BUILTIN, GPIO_OUTPUT);

    uint32_t last_time = 0;

    while (1) {
            
        for(int i = 0 ; i < 12 ; i++) {
        GPIO_Toggle(LED_BUILTIN);
        Delay(200); 
        }
         for(int i = 0 ; i < 12 ; i++) {
        GPIO_Toggle(LED_BUILTIN);
        Delay(500); 
        }
        for(int i = 0 ; i < 12 ; i++) {
        GPIO_Toggle(LED_BUILTIN);
        Delay(900); 
        }
        
    }

} 
