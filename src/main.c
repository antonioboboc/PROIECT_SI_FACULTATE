#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "bsp/nano.h"
#include <stdint.h>
#include "radar_app.h"


int main(void) {
    
    Timer0_Init();

    uint8_t distance_cm = 11;
    uint8_t radar_state;
    
    Radar_Init();

    while (1) {
            
       radar_state = Radar_return_state(distance_cm);
        if(radar_state == RADAR_STATE_WARNING) {
            Radar_WarningStateAction();
        } 
        else {
            Radar_SafeStateAction();
        }
    }

return 0;
} 
