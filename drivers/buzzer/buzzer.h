#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

void Buzzer_Init(void);
void Buzzer_Start(uint16_t frequency_hz);
void Buzzer_Stop(void);

#endif