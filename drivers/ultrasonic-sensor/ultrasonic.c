#include "ultrasonic.h"
#include "src/app_config.h"
#include "drivers/gpio/gpio.h"
#include <util/delay.h>

#define ULTRASONIC_START_TIMEOUT_US 30000UL
#define ULTRASONIC_ECHO_TIMEOUT_US 30000UL

static void trigger_pulse(void) {
    GPIO_Write(ULTRASONIC_TRIG_PIN, GPIO_LOW);
    _delay_us(2);

    GPIO_Write(ULTRASONIC_TRIG_PIN, GPIO_HIGH);
    _delay_us(10);

    GPIO_Write(ULTRASONIC_TRIG_PIN, GPIO_LOW);
}

void ultrasonic_Init(void) {
    GPIO_Init(ULTRASONIC_TRIG_PIN, GPIO_OUTPUT);
    GPIO_Init(ULTRASONIC_ECHO_PIN, GPIO_INPUT);
    GPIO_Write(ULTRASONIC_TRIG_PIN, GPIO_LOW);
}

uint16_t ultrasonic_Read(void) {
    uint32_t wait_us = 0;
    uint32_t echo_pulse_us = 0;

    trigger_pulse();

    while (GPIO_Read(ULTRASONIC_ECHO_PIN) == GPIO_LOW) {
        if (wait_us++ >= ULTRASONIC_START_TIMEOUT_US) {
            return 0;
        }
        _delay_us(1);
    }

    while (GPIO_Read(ULTRASONIC_ECHO_PIN) == GPIO_HIGH) {
        if (echo_pulse_us++ >= ULTRASONIC_ECHO_TIMEOUT_US) {
            return 0;
        }
        _delay_us(1);
    }

    return (uint16_t)(echo_pulse_us / 58UL);
}
