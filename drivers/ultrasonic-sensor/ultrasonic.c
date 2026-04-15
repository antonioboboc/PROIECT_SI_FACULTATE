#include "hcsr04.h"
#include <util/delay.h>

// CONFIGURARE PINI
#define TRIG_PORT PORTB
#define TRIG_DDR  DDRB
#define TRIG_PIN  PB1

#define ECHO_PIN_REG PINB
#define ECHO_DDR DDRB
#define ECHO_PIN PB0

// TIMER1 pentru masurare
static void Timer1_Init()
{
    TCCR1A = 0x00;
    TCCR1B = (1 << CS11); // prescaler 8 -> 0.5us per tick la 16MHz
    TCNT1 = 0;
}

void HCSR04_Init(void)
{
    // TRIG = output
    TRIG_DDR |= (1 << TRIG_PIN);

    // ECHO = input
    ECHO_DDR &= ~(1 << ECHO_PIN);

    Timer1_Init();
}

// delay ~10us
static void trigger_pulse()
{
    TRIG_PORT &= ~(1 << TRIG_PIN);
    _delay_us(2);

    TRIG_PORT |= (1 << TRIG_PIN);
    _delay_us(10);

    TRIG_PORT &= ~(1 << TRIG_PIN);
}

uint16_t HCSR04_Read(void)
{
    uint16_t time = 0;

    trigger_pulse();

    // asteapta ECHO HIGH
    while (!(ECHO_PIN_REG & (1 << ECHO_PIN)));

    // reset timer
    TCNT1 = 0;

    // asteapta ECHO LOW
    while (ECHO_PIN_REG & (1 << ECHO_PIN));

    time = TCNT1;

    // conversie:
    // tick = 0.5us → time * 0.5 = microsecunde
    // dist = us / 58
    uint16_t distance = (time / 2) / 58;

    return distance;
}