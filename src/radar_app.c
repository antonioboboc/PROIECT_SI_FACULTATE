#include "radar_app.h"
#include "drivers/gpio/gpio.h"
#include "bsp/nano.h"

uint8_t Radar_return_state(uint8_t distance_cm) {
    if (distance_cm <= RADAR_ALERT_DISTANCE_CM) {
        return RADAR_STATE_WARNING;
    } 
    else {
        return RADAR_STATE_SAFE;
    }
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
}

void Radar_WarningStateAction(void) {
    GPIO_Write(RED_LED1_PIN, GPIO_HIGH);
    GPIO_Write(RED_LED2_PIN, GPIO_HIGH);

    GPIO_Write(GREEN_LED1_PIN, GPIO_LOW);
    GPIO_Write(GREEN_LED2_PIN, GPIO_LOW);
}