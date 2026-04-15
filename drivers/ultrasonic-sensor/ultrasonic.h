#ifndef ultrasonic
#define ultrasonic

#include <avr/io.h>

void ultrasonic_Init(void);
uint16_t ultrasonic_Read(void); 

#endif