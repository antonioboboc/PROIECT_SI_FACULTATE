#include "drivers/buzzer/buzzer.h"
#include "drivers/servo/servo.h"
#include "drivers/ultrasonic-sensor/ultrasonic.h"
#include "src/app_config.h"
#include "src/radar_app.h"
#include "utils/delay.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>




int main(void) {
    uint8_t current_angle   = SERVO_MIN_ANGLE;
    int8_t  sweep_direction = 1;
    uint8_t buzzer_on       = 0U;
 
    Servo_Init();
    ultrasonic_Init();
    Radar_Init();
    Buzzer_Init();
    
    Radar_SafeStateAction();
    Servo_SetAngle(current_angle);
 
    while (1) {
 
        /* ── DETECTIE ───────────────────────────────────────────── */
        if (Radar_CheckForObstacle() == RADAR_STATE_WARNING) {
 
            Radar_WarningStateAction();
            buzzer_on = 0U;
 
            while (Radar_CheckForObstacle() == RADAR_STATE_WARNING) {
                if (buzzer_on == 0U) {
                    Buzzer_Start(BUZZER_ALERT_FREQUENCY_HZ);
                    buzzer_on = 1U;
                    Delay(BUZZER_BEEP_ON_MS);
                } else {
                    Buzzer_Stop();
                    buzzer_on = 0U;
                    Delay(BUZZER_BEEP_OFF_MS);
                }
            }
 
            /* Obiect disparut */
            Buzzer_Stop();
            buzzer_on = 0U;
            Radar_SafeStateAction();
        }
 
        /* ── MODUL SAFE — avanseaza servo ───────────────────────── */
        if (sweep_direction > 0) {
            if (current_angle >= SERVO_MAX_ANGLE) {
                sweep_direction = -1;
                current_angle   = SERVO_MAX_ANGLE;
            } else {
                current_angle += SERVO_STEP_ANGLE;
            }
        } else {
            if (current_angle <= SERVO_MIN_ANGLE) {
                sweep_direction = 1;
                current_angle   = SERVO_MIN_ANGLE;
            } else {
                current_angle -= SERVO_STEP_ANGLE;
            }
        }
 
        Servo_SetAngle(current_angle);
        Delay(SERVO_STEP_DELAY_MS);
    }
 
    return 0;
}