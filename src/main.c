#include "drivers/gpio/gpio.h"
#include "drivers/servo/servo.h"
#include "drivers/ultrasonic-sensor/ultrasonic.h"
#include "utils/delay.h"
#include "bsp/nano.h"
#include <avr/interrupt.h>
#include <stdint.h>



int main(void) {

    uint16_t distance_cm = 0;
    uint8_t radar_state = 0;
    Timer0_Init();
    Servo_Init();
    ultrasonic_Init();
    Buzzer_Init();
    sei();



    while (1) {

        / Sweep 0° → 180° in 1° steps /
          for (uint8_t angle = 0; angle <= 180; angle++) {
            Servo_SetAngle(angle);
            Delay(15);   / ~15 ms between steps for smooth motion /
        }

        / Sweep 180° → 0° in 1° steps/
        for (int16_t angle = 180; angle >= 0; angle--) {
            Servo_SetAngle((uint8_t)angle);
            Delay(15);
        }




        distance_cm = ultrasonic_Read();


        radar_state = Radar_return_state(distance_cm);
        if(radar_state == RADAR_STATE_WARNING) {
            Radar_WarningStateAction();
        } 
        else {
            Radar_SafeStateAction();
        }
        Delay(100);
    }

    return 0;
}