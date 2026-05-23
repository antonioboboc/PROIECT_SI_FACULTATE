#include "radar_app.h"
#include "drivers/gpio/gpio.h"
#include "bsp/nano.h"
#include "ultrasonic.h"
#include "buzzer.h"  
#include "lcd.h"


uint8_t Radar_return_state(uint8_t distance_cm) {
    if (distance_cm <= RADAR_ALERT_DISTANCE_CM) {
        return RADAR_STATE_WARNING;
    } 
    else {
        return RADAR_STATE_SAFE;
    }
}

 uint8_t Radar_CheckForObstacle(void) {
         uint16_t distance_cm = ultrasonic_Read();
     return Radar_return_state((uint8_t)distance_cm);
 }

void  Radar_Init(void) {
    GPIO_Init(RED_LED1_PIN, GPIO_OUTPUT);
    GPIO_Init(RED_LED2_PIN, GPIO_OUTPUT);

    GPIO_Init(GREEN_LED1_PIN, GPIO_OUTPUT);
    GPIO_Init(GREEN_LED2_PIN, GPIO_OUTPUT);
}

void  Radar_SafeStateAction(void) {
    GPIO_Write(GREEN_LED1_PIN, GPIO_HIGH);
    GPIO_Write(GREEN_LED2_PIN, GPIO_HIGH);
   

    GPIO_Write(RED_LED1_PIN, GPIO_LOW);
    GPIO_Write(RED_LED2_PIN, GPIO_LOW);
    Buzzer_Stop();

    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("Niciun obiect");
}

void Radar_WarningStateAction(void) {
    GPIO_Write(RED_LED1_PIN, GPIO_HIGH);
    GPIO_Write(RED_LED2_PIN, GPIO_HIGH);
    Buzzer_Start(BUZZER_ALERT_FREQUENCY_HZ);

    GPIO_Write(GREEN_LED1_PIN, GPIO_LOW);
    GPIO_Write(GREEN_LED2_PIN, GPIO_LOW);
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("Obiect detectat!");

}