#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "drivers/buzzer/buzzer.h"
#include "bsp/nano.h"
#include "delay.h"
#include <stdint.h>
#include "radar_app.h"
#include "drivers/servo/servo.h"
#include "utils/delay.h"


int main(void) {

    uint8_t radar_state;
    uint8_t distance_cm = 10; 

    Timer0_Init();   /* system tick for Delay() */
    Servo_Init();    /* D9 / OC1A – 50 Hz, starts at 90° */
    uint8_t radar_state;
    uint16_t distance_cm=0;
	
    while (1) {


        radar_state = Radar_return_state(distance_cm);
        if(radar_state == RADAR_STATE_WARNING) {
            Radar_WarningStateAction();
        } 
        else {
            Radar_SafeStateAction();
        }

		
        for (uint8_t angle = 0; angle <= 180; angle++) {
            Servo_SetAngle(angle);
            Delay(15);   /* ~15 ms between steps for smooth motion */
        }

        /* Sweep 180° → 0° in 1° steps */
        for (int16_t angle = 180; angle >= 0; angle--) {
            Servo_SetAngle((uint8_t)angle);
            Delay(15);
        }
    }

    return 0;
}
