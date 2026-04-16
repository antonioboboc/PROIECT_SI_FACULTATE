#include "drivers/buzzer/buzzer.h"
#include "drivers/servo/servo.h"
#include "drivers/ultrasonic-sensor/ultrasonic.h"
#include "src/app_config.h"
#include "src/radar_app.h"
#include "utils/delay.h"
#include <stdint.h>

static uint8_t Radar_CheckForObstacle(void) {
    uint16_t distance_cm = ultrasonic_Read();
    return Radar_return_state((uint8_t)distance_cm);
}

int main(void) {
    uint8_t current_angle = SERVO_MIN_ANGLE;
    int8_t sweep_direction = 1;
    uint8_t buzzer_enabled = 0;

    Servo_Init();
    ultrasonic_Init();
    Radar_Init();
    Servo_SetAngle(current_angle);

    while (1) {
        if (Radar_CheckForObstacle() == RADAR_STATE_WARNING) {
            Radar_WarningStateAction();

            while (Radar_CheckForObstacle() == RADAR_STATE_WARNING) {
                if (buzzer_enabled == 0U) {
                    Radar_WarningStateAction();
                    buzzer_enabled = 1U;
                    Delay(BUZZER_BEEP_ON_MS);
                } else {
                    Buzzer_Stop();
                    buzzer_enabled = 0U;
                    Delay(BUZZER_BEEP_OFF_MS);
                }
            }

            Buzzer_Stop();
            buzzer_enabled = 0U;
            Radar_SafeStateAction();
            continue;
        }

        Radar_SafeStateAction();
        Servo_SetAngle(current_angle);
        Delay(SERVO_STEP_DELAY_MS);

        if (sweep_direction > 0) {
            if (current_angle >= SERVO_MAX_ANGLE) {
                sweep_direction = -1;
            } else {
                current_angle += SERVO_STEP_ANGLE;
            }
        } else {
            if (current_angle <= SERVO_MIN_ANGLE) {
                sweep_direction = 1;
            } else {
                current_angle -= SERVO_STEP_ANGLE;
            }
        }
    }

    return 0;
}