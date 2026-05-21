#include "i2c.h"
#include <avr/io.h>

#define I2C_FREQ 100000UL

void I2C_Init(void)
{
    /*
     * SCL frequency:
     * SCL = F_CPU / (16 + 2 * TWBR * prescaler)
     *
     * Pentru F_CPU = 16 MHz si I2C = 100 kHz:
     * TWBR = 72, prescaler = 1
     */

    TWSR = 0x00;   // prescaler = 1
    TWBR = 72;     // 100 kHz la 16 MHz
    TWCR = (1 << TWEN);
}

void I2C_Start(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT))) {
        ;
    }
}

void I2C_Stop(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void I2C_Write(uint8_t data)
{
    TWDR = data;

    TWCR = (1 << TWINT) | (1 << TWEN);

    while (!(TWCR & (1 << TWINT))) {
        ;
    }
}