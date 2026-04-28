#include "ultrasonic.h"
#include "src/app_config.h"
#include "drivers/gpio/gpio.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define ULTRASONIC_START_TIMEOUT_US 30000UL
#define ULTRASONIC_ECHO_TIMEOUT_US 30000UL

static volatile uint8_t ultrasonic_waiting_rise = 0;
static volatile uint8_t ultrasonic_waiting_fall = 0;
static volatile uint8_t ultrasonic_measurement_ready = 0;
static volatile uint8_t ultrasonic_measurement_timeout = 0;
static volatile uint16_t ultrasonic_start_ticks = 0;
static volatile uint16_t ultrasonic_echo_ticks = 0;
static volatile uint8_t ultrasonic_last_echo_level = 0;

static void trigger_pulse(void) {
    GPIO_Write(ULTRASONIC_TRIG_PIN, GPIO_LOW);
    _delay_us(2);

    GPIO_Write(ULTRASONIC_TRIG_PIN, GPIO_HIGH);
    _delay_us(10);

    GPIO_Write(ULTRASONIC_TRIG_PIN, GPIO_LOW);
}

static uint16_t ultrasonic_ticks_elapsed(uint16_t start, uint16_t end) {
    uint16_t top = ICR1;

    if (end >= start) {
        return (uint16_t)(end - start);
    }

    return (uint16_t)((top - start) + end + 1U);
}

void ultrasonic_Init(void) {
    GPIO_Init(ULTRASONIC_TRIG_PIN, GPIO_OUTPUT);
    GPIO_Init(ULTRASONIC_ECHO_PIN, GPIO_INPUT);
    GPIO_Write(ULTRASONIC_TRIG_PIN, GPIO_LOW);

    ultrasonic_last_echo_level = (uint8_t)GPIO_Read(ULTRASONIC_ECHO_PIN);

    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT3);
    sei();
}

uint16_t ultrasonic_Read(void) {
    uint32_t wait_us = 0;
    uint32_t echo_pulse_us = 0;

    ultrasonic_waiting_rise = 1;
    ultrasonic_waiting_fall = 0;
    ultrasonic_measurement_ready = 0;
    ultrasonic_measurement_timeout = 0;

    trigger_pulse();

    while (!ultrasonic_waiting_fall) {
        if (wait_us++ >= ULTRASONIC_START_TIMEOUT_US) {
            ultrasonic_waiting_rise = 0;
            ultrasonic_waiting_fall = 0;
            return 0;
        }
        _delay_us(1);
    }

    wait_us = 0;
    while (!ultrasonic_measurement_ready) {
        if (ultrasonic_measurement_timeout) {
            ultrasonic_waiting_rise = 0;
            ultrasonic_waiting_fall = 0;
            return 0;
        }

        if (wait_us++ >= ULTRASONIC_ECHO_TIMEOUT_US) {
            ultrasonic_waiting_rise = 0;
            ultrasonic_waiting_fall = 0;
            return 0;
        }
        _delay_us(1);
    }

    echo_pulse_us = ultrasonic_echo_ticks / 2UL;
    return (uint16_t)(echo_pulse_us / 58UL);
}

ISR(PCINT0_vect) {
    uint8_t echo_level = (uint8_t)GPIO_Read(ULTRASONIC_ECHO_PIN);

    if (echo_level == ultrasonic_last_echo_level) {
        return;
    }

    ultrasonic_last_echo_level = echo_level;

    if (ultrasonic_waiting_rise && (echo_level == GPIO_HIGH)) {
        ultrasonic_start_ticks = TCNT1;
        ultrasonic_waiting_rise = 0;
        ultrasonic_waiting_fall = 1;
        return;
    }

    if (ultrasonic_waiting_fall && (echo_level == GPIO_LOW)) {
        uint16_t end_ticks = TCNT1;
        ultrasonic_echo_ticks = ultrasonic_ticks_elapsed(ultrasonic_start_ticks, end_ticks);
        ultrasonic_waiting_fall = 0;
        ultrasonic_measurement_ready = 1;

        if (ultrasonic_echo_ticks >= (uint16_t)(ULTRASONIC_ECHO_TIMEOUT_US * 2UL)) {
            ultrasonic_measurement_timeout = 1;
        }
    }
}