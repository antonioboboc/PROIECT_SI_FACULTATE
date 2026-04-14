#include "drivers/gpio/gpio.h"
#include "drivers/timer/timer0.h"
#include "drivers/buzzer/buzzer.h"
#include "bsp/nano.h"
#include "delay.h"
#include <stdint.h>
#include "radar_app.h"


int main(void) {
    Timer0_Init();
    Buzzer_Init();

    while (1) {
        Buzzer_Start(500);
        Delay(300);
        Buzzer_Stop();
        Delay(200);

        Buzzer_Start(1000);
        Delay(300);
        Buzzer_Stop();
        Delay(200);

        Buzzer_Start(2000);
        Delay(300);
        Buzzer_Stop();
        Delay(500);
    }

    return 0;
}
