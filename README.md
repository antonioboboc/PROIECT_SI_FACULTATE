# Smart Radar Surveillance System — ATmega328P

A bare-metal embedded system that monitors the surrounding area by scanning it with an **HC-SR04 ultrasonic sensor** mounted on an **SG90 servo motor** — all running on an ATmega328P (Arduino Nano / Uno) at 16 MHz, written entirely in C with no Arduino libraries.

---

## Table of Contents

- [Overview](#overview)
- [How It Works](#how-it-works)
- [Hardware Requirements](#hardware-requirements)
- [Pin Mapping](#pin-mapping)
- [Project Structure](#project-structure)
- [Driver Reference](#driver-reference)
- [Build & Flash](#build--flash)
- [Running Tests](#running-tests)
- [Contributors](#contributors)

---

## Overview

The system rotates an HC-SR04 ultrasonic sensor across a 180° arc using an SG90 servo motor.
At each step, the sensor measures the distance to any object in front of it.
If an obstacle is detected within the predefined safety threshold, the system switches to alert mode.

```
        SG90 Servo (D9)
              │
              ▼
      ┌───────────────┐   distance_cm   ┌──────────────────┐
      │  HC-SR04      │ ─────────────► │  Radar Logic     │
      │  Ultrasonic   │                 │  (radar_app.c)   │
      └───────────────┘                 └────────┬─────────┘
                                                 │
                              ┌──────────────────┴──────────────────┐
                              │                                     │
                     distance > 20 cm                     distance ≤ 20 cm
                     (SAFE STATE)                         (WARNING STATE)
                              │                                     │
                    ┌─────────▼──────────┐             ┌────────────▼──────────┐
                    │ Green LEDs ON      │             │ Red LEDs ON           │
                    │ Red LEDs OFF       │             │ Green LEDs OFF        │
                    │ Buzzer OFF         │             │ Buzzer ON (2 kHz)     │
                    │ LCD: "No object"   │             │ LCD: "Object detected"│
                    └────────────────────┘             └───────────────────────┘
```

---

## How It Works

### Scanning Mechanism

The servo motor sweeps the ultrasonic sensor step by step from **0° to 180°** and back, advancing **1° every 15 ms**.
At each position, a distance measurement is triggered.

### Distance Measurement — HC-SR04

The sensor fires a **10 µs trigger pulse**; the firmware measures the echo pulse width using a pin-change interrupt (PCINT0) and Timer1, then converts it to centimetres:

```
distance_cm = echo_pulse_µs / 58
```

A timeout of **30 000 µs** is applied on both the rising and falling edge waits to prevent the system from locking up when no echo is received.

### Safe State

When no object is detected within the safety threshold:

- Green LEDs are turned **ON**
- Red LEDs are turned **OFF**
- Buzzer is **silent**
- LCD displays `"No object"`

### Warning State

When an object is detected closer than `RADAR_ALERT_DISTANCE_CM` (20 cm):

- Red LEDs are turned **ON**
- Green LEDs are turned **OFF**
- Buzzer emits a **2 kHz beep** pattern (100 ms ON / 100 ms OFF)
- LCD displays `"Object detected!"`

The system remains in warning state and keeps beeping for as long as the obstacle is present. Once the object moves away, it automatically returns to safe state and resumes scanning.

---

## Hardware Requirements

| Component | Part | Details |
|-----------|------|---------|
| MCU | ATmega328P | Arduino Nano or Uno, 16 MHz |
| Servo | SG90 | Signal → D9, 5 V, GND |
| Ultrasonic sensor | HC-SR04 | TRIG → D10, ECHO → D11 |
| LCD display | 1602 + PCF8574 I2C adapter | I2C address 0x27, SDA → A4, SCL → A5 |
| Red LED × 2 | — | D7, D4 — obstacle warning |
| Green LED × 2 | — | D5, D6 — area clear indicator |
| Buzzer (passive) | — | D3 / OC2B (Timer2) |
| Resistors | — | Current limiting for LEDs |
| Power supply | — | 5 V via USB or external rail |

> **Power note:** The SG90 servo can draw up to 700 mA under load. When powering from USB, keep mechanical load minimal or use an external 5 V rail with a shared GND.

---

## Pin Mapping

### Servo

| Arduino Pin | AVR Pin | Function |
|-------------|---------|----------|
| D9 | PB1 / OC1A | SG90 PWM signal (50 Hz, Timer1) |

### HC-SR04 Ultrasonic Sensor

| Arduino Pin | AVR Pin | Signal |
|-------------|---------|--------|
| D10 | PB2 | TRIG |
| D11 | PB3 / PCINT3 | ECHO (pin-change interrupt) |

### LEDs

| Arduino Pin | AVR Pin | Function |
|-------------|---------|----------|
| D7 | PD7 | Red LED 1 |
| D4 | PD4 | Red LED 2 |
| D5 | PD5 | Green LED 1 |
| D6 | PD6 | Green LED 2 |

### Buzzer

| Arduino Pin | AVR Pin | Function |
|-------------|---------|----------|
| D3 | PD3 / OC2B | Passive buzzer (Timer2 PWM) |

### I2C LCD

| Arduino Pin | AVR Pin | Function |
|-------------|---------|----------|
| A4 | PC4 / SDA | I2C data |
| A5 | PC5 / SCL | I2C clock |

---

## Project Structure

```
PROIECT_SI_FACULTATE/
├── src/
│   ├── main.c              # Application entry point — servo sweep & obstacle detection loop
│   ├── radar_app.c         # Radar logic — state machine, LED/LCD/buzzer actions
│   ├── radar_app.h         # Radar API declarations
│   └── app_config.h        # Central configuration — pin assignments, thresholds, timings
├── drivers/
│   ├── adc/                # 10-bit ADC driver (AVCC reference, prescaler 128)
│   ├── buzzer/             # Passive buzzer driver (built on PWM driver, Timer2 OC2B)
│   ├── eeprom/             # EEPROM read / write / update driver
│   ├── gpio/               # GPIO input / output / toggle driver
│   ├── i2c/                # TWI hardware driver (100 kHz)
│   ├── interrupt/          # External interrupt driver (INT0, INT1)
│   ├── lcd/                # 1602 LCD driver over I2C / PCF8574, 4-bit mode
│   ├── pwm/                # Generic PWM driver (Timer1 + Timer2)
│   │                       #   PWM_SetDutyCycle accepts pulse width in µs
│   ├── servo/              # SG90 servo driver (built on PWM driver, Timer1 OC1A)
│   ├── timer/
│   │   ├── timer0.c/h      # System tick — 1 ms CTC ISR → Millis()
│   │   ├── timer1.c/h      # 16-bit Fast PWM (Mode 14, ICR1 as TOP)
│   │   └── timer2.c/h      # 8-bit Fast PWM (Mode 3, TOP = 0xFF)
│   └── ultrasonic-sensor/  # HC-SR04 driver — PCINT0 echo timing via Timer1
├── bsp/
│   ├── bsp.h               # Board selector (BOARD_NANO / BOARD_UNO)
│   ├── nano.h              # Arduino Nano pin definitions
│   └── uno.h               # Arduino Uno pin definitions
├── utils/
│   ├── delay.c/h           # Millisecond delay (Millis-based, non-blocking friendly)
│   └── utils.h             # Bit manipulation macros (SET_BIT, CLEAR_BIT, TOGGLE_BIT…)
├── test/                   # Unit tests (host-compiled with register mocks)
│   ├── framework/
│   │   └── test_framework.h
│   ├── mocks/
│   │   ├── avr/            # AVR register mocks for host compilation
│   │   └── registers.c
│   ├── test_gpio.c
│   ├── test_pwm.c
│   └── test_pwm_wrapper.c
└── Makefile
```

---

## Driver Reference

### Radar App (`src/`)

```c
Radar_Init();               // Configure LED pins as outputs
Radar_CheckForObstacle();   // Trigger ultrasonic read → returns RADAR_STATE_SAFE or RADAR_STATE_WARNING
Radar_SafeStateAction();    // Green LEDs ON, red OFF, buzzer OFF, LCD "No object"
Radar_WarningStateAction(); // Red LEDs ON, green OFF, buzzer ON, LCD "Object detected!"
```

**State constants (`app_config.h`):**

| Constant | Value | Meaning |
|----------|-------|---------|
| `RADAR_STATE_SAFE` | 0 | No obstacle within threshold |
| `RADAR_STATE_WARNING` | 1 | Obstacle closer than 20 cm |
| `RADAR_ALERT_DISTANCE_CM` | 20 | Detection threshold in cm |

---

### Servo (`drivers/servo/`)

```c
Servo_Init();              // Init Timer1 at 50 Hz on D9, move to 0°
Servo_SetAngle(uint8_t);   // 0–180°, clamped — ~4 000 positions (16-bit Timer1)
Servo_Stop();              // Disable PWM output
```

**Pulse mapping:**

| Angle | Pulse width | OCR1A ticks (prescaler = 8) |
|-------|-------------|------------------------------|
| 0°    | 500 µs      | 1 000                        |
| 90°   | 1 500 µs    | 3 000                        |
| 180°  | 2 500 µs    | 5 000                        |

**Sweep parameters (`app_config.h`):**

| Constant | Value | Meaning |
|----------|-------|---------|
| `SERVO_MIN_ANGLE` | 0° | Start of sweep |
| `SERVO_MAX_ANGLE` | 180° | End of sweep |
| `SERVO_STEP_ANGLE` | 1° | Increment per step |
| `SERVO_STEP_DELAY_MS` | 15 ms | Delay between steps |

---

### Ultrasonic (`drivers/ultrasonic-sensor/`)

```c
ultrasonic_Init();    // Configure TRIG/ECHO pins, enable PCINT0 interrupt
uint16_t d = ultrasonic_Read();  // Trigger pulse → return distance in cm (0 on timeout)
```

The echo timing uses **Timer1's free-running counter (TCNT1)** captured inside the PCINT0 ISR, so no dedicated timer is consumed. Tick-to-microsecond conversion:

```
echo_pulse_µs = ticks / 2     (Timer1 at 16 MHz, prescaler = 8 → 0.5 µs / tick)
distance_cm   = echo_pulse_µs / 58
```

---

### Buzzer (`drivers/buzzer/`)

```c
Buzzer_Init();                         // No-op — pin configured by PWM on first start
Buzzer_Start(uint16_t frequency_hz);   // Generate tone on D3 via Timer2 OC2B
Buzzer_Stop();                         // Silence output, disable Timer2 OC2B
```

Alert tone is generated at **2 000 Hz** with a **50% duty cycle** (500 µs pulse).
The beep pattern in warning state: **100 ms ON / 100 ms OFF**.

---

### LCD (`drivers/lcd/`)

```c
LCD_Init();                          // I2C init + HD44780 4-bit init sequence
LCD_Clear();                         // Clear display, cursor home (2 ms delay)
LCD_SetCursor(uint8_t row, uint8_t col); // row 0 or 1, col 0–15
LCD_Print(const char *text);         // Write null-terminated string
LCD_PrintNumber(uint16_t number);    // Write unsigned integer
```

Uses the TWI hardware peripheral at **100 kHz** via the PCF8574 I2C expander (address `0x27`).

---

### PWM (`drivers/pwm/`)

```c
PWM_Init(port, pin, freq_hz);           // Configure timer for desired frequency
PWM_SetDutyCycle(port, pin, pulse_us);  // Set pulse width in microseconds
PWM_Stop(port, pin);                    // Disable output
```

The prescaler is read from the hardware registers (`TCCR1B` / `TCCR2B`) at runtime, so `PWM_SetDutyCycle` stays accurate for any frequency set at init.

> Timer1 (D9, D10) supports exact 50 Hz — used for servo.  
> Timer2 (D3, D11) is 8-bit; closest achievable to 50 Hz is ~61 Hz — used for buzzer.

---

### ADC (`drivers/adc/`)

```c
ADC_Init();                    // AVCC reference, prescaler 128 (125 kHz @ 16 MHz)
uint16_t v = ADC_Read(ch);     // Blocking 10-bit read, returns 0–1023
```

---

### Timer0 / Millis (`drivers/timer/timer0/`)

```c
Timer0_Init();          // CTC mode, 1 ms tick, enables global interrupts
uint32_t t = Millis();  // Milliseconds since startup (atomic 32-bit read)
```

---

## Build & Flash

**Prerequisites:** `avr-gcc`, `avr-libc`, `avrdude`

```bash
# Build for Arduino Nano (default)
make

# Build for Arduino Uno
make BOARD=uno

# Flash via USB — default port is COM5, adjust as needed
make flash

# Flash on Linux / macOS
make flash PORT=/dev/ttyUSB0

# Clean build artefacts
make clean
```

---

## Contributors

- **Bizera Razvan Ionut**
- **Boboc Antonio Valentin**
