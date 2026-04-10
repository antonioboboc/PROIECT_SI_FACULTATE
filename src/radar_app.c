#include "radar_app.h"

uint8_t Radar_return_state(uint8_t distance_cm) {
    if (distance_cm <= RADAR_ALERT_DISTANCE_CM) {
        return RADAR_STATE_WARNING;
    } 
    else {
        return RADAR_STATE_SAFE;
    }
}