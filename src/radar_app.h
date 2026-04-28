#ifndef RADAR_APP_H
#define RADAR_APP_H

#include "app_config.h"
#include <stdint.h>

uint8_t Radar_CheckForObstacle(void);
uint8_t Radar_return_state(uint8_t distance_cm);
void  Radar_SafeStateAction(void);
void  Radar_WarningStateAction(void);
void  Radar_Init(void);

#endif