# Smart Radar Surveillance System

Smart Radar Surveillance System is an embedded application based on the ATmega328P microcontroller, designed to monitor the surrounding area by scanning it with an ultrasonic sensor mounted on a servo motor. The project is developed on the Arduino Nano platform used during the Embedded Systems laboratory and focuses on obstacle detection, visual warning, audible alert signaling, and real-time status display through an I2C LCD.

🚀 Key Features  
Direct Register Programming: The application is implemented using low-level register access for better hardware understanding and precise control of peripherals.  
Radar Scanning Mechanism: A servo motor rotates the ultrasonic sensor across multiple angles in order to scan the environment.  
Obstacle Detection: The system measures distance using an ultrasonic sensor and determines whether an object is present inside a predefined safety zone.  
I2C Display Integration: Uses a 1602 LCD with a PCF8574 I2C adapter to display real-time system status, obstacle alerts, and measured distance information.  
Visual Warning System: Two red LEDs are activated when an obstacle is detected, while two green LEDs remain active when the area is safe.  
Audible Alert: A buzzer emits a sound whenever the system detects an obstacle inside the critical distance.  
Modular Software Architecture: The project is structured into reusable drivers and application-level modules.  

🛠 Hardware Components  
ATmega328P microcontroller  
Arduino Nano  
HC-SR04 ultrasonic sensor  
SG90 servo motor  
1602 LCD display with I2C adapter (PCF8574)  
2 red LEDs  
2 green LEDs  
Buzzer  
Resistors  
Jumper wires  
Breadboard  

⚙ Operating Principle  
The system works by continuously rotating the ultrasonic sensor with the help of a servo motor and performing distance measurements at different angles.

Safe State  
When no object is detected inside the predefined safety distance, the monitored area is considered safe. In this state, the LCD displays a safe message, the two green LEDs remain turned on, the red LEDs stay off, and the buzzer is inactive.

Alert State  
When an object is detected closer than the defined safety threshold, the system switches to alert mode. In this state, the LCD displays a warning message, the two red LEDs are turned on, the green LEDs are turned off, and the buzzer emits an alarm signal.

Scanning Process  
The servo motor moves the ultrasonic sensor step by step across the monitored area. For each position, the sensor performs a distance measurement. Based on the measured value, the application updates the current system state and activates the proper warning signals.

📡 System Output  
The system provides real-time feedback through:

LCD status messages  
visual signaling using red and green LEDs  
audible warning using a buzzer  

Optional future extensions may include transmitting measured distance and servo angle through UART for debugging or monitoring purposes.

📂 Software Structure  
The application is organized into separate modules for clarity and scalability:



🎯 Project Goal  
The main objective of this project is to implement a modular embedded application on ATmega328P that demonstrates:

sensor interfacing  
servo motor control  
distance measurement  
real-time obstacle detection  
visual and audible alert generation  
clean project organization for collaborative development  

👥 Authors  
Boboc Antonio Valentin  
Bizera Razvan Ionut
