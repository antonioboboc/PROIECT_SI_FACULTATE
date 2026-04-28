 #include "drivers/buzzer/buzzer.h"
 #include "drivers/servo/servo.h"
 #include "drivers/ultrasonic-sensor/ultrasonic.h"
 #include "src/app_config.h"
 #include "src/radar_app.h"
 #include "utils/delay.h"
 #include <stdint.h>
 #include <avr/io.h>
 #include <avr/interrupt.h>


// int main(void) {
//     uint8_t current_angle   = SERVO_MIN_ANGLE;
//     int8_t  sweep_direction = 1;
//     uint8_t buzzer_on       = 0U;
 
//     Servo_Init();
//     ultrasonic_Init();
//     Radar_Init();
//     Buzzer_Init();
    
//     Radar_SafeStateAction();
//     Servo_SetAngle(current_angle);
 
//     while (1) {
 
//         /* ── DETECTIE ───────────────────────────────────────────── */
//         if (Radar_CheckForObstacle() == RADAR_STATE_WARNING) {
 
//             Radar_WarningStateAction();
//             buzzer_on = 0U;
 
//             while (Radar_CheckForObstacle() == RADAR_STATE_WARNING) {
//                 if (buzzer_on == 0U) {
//                     Buzzer_Start(BUZZER_ALERT_FREQUENCY_HZ);
//                     buzzer_on = 1U;
//                     Delay(BUZZER_BEEP_ON_MS);
//                 } else {
//                     Buzzer_Stop();
//                     buzzer_on = 0U;
//                     Delay(BUZZER_BEEP_OFF_MS);
//                 }
//             }
 
//             /* Obiect disparut */
//             Buzzer_Stop();
//             buzzer_on = 0U;
//             Radar_SafeStateAction();
//         }
 
//         /* ── MODUL SAFE — avanseaza servo ───────────────────────── */
//         if (sweep_direction > 0) {
//             if (current_angle >= SERVO_MAX_ANGLE) {
//                 sweep_direction = -1;
//                 current_angle   = SERVO_MAX_ANGLE;
//             } else {
//                 current_angle += SERVO_STEP_ANGLE;
//             }
//         } else {
//             if (current_angle <= SERVO_MIN_ANGLE) {
//                 sweep_direction = 1;
//                 current_angle   = SERVO_MIN_ANGLE;
//             } else {
//                 current_angle -= SERVO_STEP_ANGLE;
//             }
//         }
 
//         Servo_SetAngle(current_angle);
//         Delay(SERVO_STEP_DELAY_MS);
//     }
 
//     return 0;
// }
#include "drivers/i2c/i2c.h"
#include "drivers/lcd/lcd.h"
#include "utils/delay.h"
#include <stdint.h>

int main(void)
{
    uint16_t counter = 0;

    I2C_Init();
    LCD_Init();

    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("LCD I2C test");

    LCD_SetCursor(1, 0);
    LCD_Print("Counter: 0");

    while (1)
    {
        LCD_SetCursor(1, 9);
        LCD_PrintNumber(counter);

        counter++;

        Delay(1000);

        if (counter > 9999)
        {
            counter = 0;
            LCD_Clear();
            LCD_SetCursor(0, 0);
            LCD_Print("LCD I2C test");
            LCD_SetCursor(1, 0);
            LCD_Print("Counter: ");
        }
    }

    return 0;
}
