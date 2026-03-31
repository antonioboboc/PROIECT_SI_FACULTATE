# Smart Temperature Control System

Smart Temperature Control System is an embedded application based on the ATmega328P microcontroller, designed to monitor temperature and control a PWM output in both automatic and manual operating modes.
The project is developed as a port of a previous STM32-based idea, adapted to the ATmega328P platform used during the Embedded Systems laboratory. 

## 🚀 Key Features
- **Direct Register Programming**: The application is implemented using low-level register access for better hardware understanding and precise control of peripherals.
- **Temperature Acquisition**: Reads temperature data from an analog sensor using the ADC peripheral.
- **Automatic PWM Control**: Adjusts the PWM duty cycle proportionally to the measured temperature.
- **Manual Mode**: Allows the user to override the automatic control and set the PWM duty cycle using a potentiometer.
- **Mode Switching with Button**: A push button is used to switch between automatic and manual operating modes.
- **Serial Monitoring**: Sends system status, measured temperature, and PWM duty cycle through UART for debugging and visualization.
- **Modular Software Architecture**: The project is structured into reusable drivers and application-level modules.

## 🛠 Hardware Components
- ATmega328P microcontroller
- NTC thermistor
- Potentiometer
- Push button
- PWM-controlled  LED
- UART serial connection for monitoring

## ⚙ Operating Principle
The system works in two main modes:

### Automatic Mode
In automatic mode, the microcontroller periodically reads the temperature from the analog sensor and updates the PWM duty cycle according to the measured value. A higher temperature produces a higher duty cycle, which can be used to increase fan speed or LED intensity.

### Manual Mode
In manual mode, the temperature reading no longer controls the PWM output. Instead, the duty cycle is adjusted using a potentiometer, allowing the user to manually set the output level.

### Mode Switching
A button is used to toggle between automatic and manual mode during runtime.

## 📡 Serial Output
The system transmits relevant information through UART, such as:
- current operating mode
- measured temperature
- PWM duty cycle

This output can be viewed in a serial terminal or in a simple desktop monitoring application.

## 📂 Software Structure
The application is organized into separate modules for clarity and scalability:

- **GPIO driver** – digital input/output configuration
- **ADC driver** – analog signal acquisition
- **PWM driver** – duty cycle generation for fan or LED control
- **UART driver** – serial communication
- **Button module** – operating mode switching
- **Application logic** – coordination of all software components

## 🎯 Project Goal
The main objective of this project is to implement a modular embedded application on ATmega328P that demonstrates:
- analog data acquisition
- PWM signal generation
- manual and automatic control logic
- serial data monitoring
- clean project organization for collaborative development

## 👥 Authors
- Boboc Antonio Valentin
- Bizera Razvan Ionut
