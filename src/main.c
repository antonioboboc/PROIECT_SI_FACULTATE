#include "drivers/gpio/gpio.h"
#include "drivers/ultrasonic-sensor/ultrasonic.h"
#include "utils/delay.h"
#include "bsp/nano.h"
#include <stdint.h>

#define LED_DISTANCE_THRESHOLD_CM 20U

int main(void) {
    uint16_t distance_cm = 0;

    ultrasonic_Init();
    GPIO_Init(LED_BUILTIN, GPIO_OUTPUT);
    GPIO_Write(LED_BUILTIN, GPIO_LOW);

    while (1) {
        distance_cm = ultrasonic_Read();

        if ((distance_cm > 0U) && (distance_cm < LED_DISTANCE_THRESHOLD_CM)) {
            GPIO_Write(LED_BUILTIN, GPIO_HIGH);
        } else {
            GPIO_Write(LED_BUILTIN, GPIO_LOW);
        }

        Delay(50);
    }

    return 0;
}
