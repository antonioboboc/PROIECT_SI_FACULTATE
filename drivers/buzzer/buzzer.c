#include "buzzer.h"
#include "drivers/pwm/pwm.h"
#include "drivers/gpio/gpio.h"

void Buzzer_Init(void) {
    /* Nu e nevoie de configurare separată aici.
       Pinul este configurat de PWM când pornim buzzerul. */
}

void Buzzer_Start(uint16_t frequency_hz) {
    if (frequency_hz == 0) {
        return;
    }

    /* D3 = PD3 = Timer2 OC2B */
    PWM_Init(GPIO_PORTD, 3, frequency_hz);

    /* Pentru buzzer pasiv vrem aproximativ 50% duty cycle.
       pulse_us = jumatate din perioada = 500000 / frequency_hz */
    uint16_t pulse_us = (uint16_t)(500000UL / frequency_hz);

    PWM_SetDutyCycle(GPIO_PORTD, 3, pulse_us);
}

void Buzzer_Stop(void) {
    PWM_Stop(GPIO_PORTD, 3);
}