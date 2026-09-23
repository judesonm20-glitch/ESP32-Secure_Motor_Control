# ESP32 Secure Motor Control System

An ESP32-based access control and motor control project that uses a 4x4 keypad, OLED display, and buzzer to authenticate a user before allowing equipment to operate.

The project is being developed in stages, beginning with the security and authentication system and later expanding to motor/fan control, timer operation, emergency-stop protection, persistent password storage, and PCB implementation.

---

## Project Goal

The goal of this project is to create a secure embedded control system where a user must enter a valid access code before a motor or other connected equipment can be activated.

The system provides visual feedback through an OLED display and audio feedback through a buzzer.

---

## Current Features

- ESP32 microcontroller
- 4x4 matrix keypad
- SSD1306 OLED display
- Buzzer feedback
- User-defined access code
- Hidden password entry
- Access Granted notification
- Access Denied notification
- Ability to change the access code
- Current-code verification before changing password
- Serial Monitor debugging
- Physical breadboard prototype

---

## Hardware Components

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 Development Board | 1 | Main microcontroller |
| 4x4 Matrix Keypad | 1 | User input |
| SSD1306 OLED Display | 1 | User interface |
| Buzzer | 1 | Audio feedback |
| Breadboard | 1 | Prototype construction |
| Jumper Wires | Several | Electrical connections |
| Resistor(s) | As required | Circuit protection/biasing |

---

## Hardware Connections

### 4x4 Keypad

| Keypad Connection | ESP32 GPIO |
|---|---:|
| Row 1 | GPIO 13 |
| Row 2 | GPIO 14 |
| Row 3 | GPIO 16 |
| Row 4 | GPIO 17 |
| Column 1 | GPIO 18 |
| Column 2 | GPIO 19 |
| Column 3 | GPIO 25 |
| Column 4 | GPIO 26 |

### OLED Display

| OLED Pin | ESP32 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

### Buzzer

| Buzzer Connection | ESP32 |
|---|---|
| Signal | GPIO 27 |
| GND | GND |

---

## Communication Protocols

### I2C

The SSD1306 OLED communicates with the ESP32 using the I2C communication protocol.

- SDA: GPIO 21
- SCL: GPIO 22
- OLED Address: `0x3C`

I2C allows the ESP32 to communicate with peripheral devices using only two communication lines.

### Matrix Keypad Scanning

The 4x4 keypad uses four row connections and four column connections.

The ESP32 scans the rows and columns to determine which key the user pressed.

---

## System Operation

1. The ESP32 powers on.
2. The OLED displays the security-system startup message.
3. The OLED displays `ENTER CODE`.
4. The user enters an access code using the keypad.
5. Each entered digit is hidden using `*`.
6. The user presses `#` to submit the code.
7. The ESP32 compares the entered code with the stored access code.
8. If the code is correct:
   - OLED displays `ACCESS GRANTED`
   - Buzzer produces confirmation feedback
9. If the code is incorrect:
   - OLED displays `ACCESS DENIED`
   - Buzzer produces an error indication
10. The user can press `A` to begin the password-change process.

---

## Keypad Controls

| Key | Function |
|---|---|
| `0-9` | Enter digits |
| `#` | Enter / Submit |
| `*` | Clear / Cancel |
| `A` | Change access code |

The `B`, `C`, and `D` keys are currently reserved for future functionality.

---

## Default Access Code

The current default access code is:

`1234`

Example:

`1 2 3 4 #`

The `#` key submits the code.

---

## Changing the Access Code

Press:

`A`

The system will ask for the current access code.

Enter:

`1234 #`

The user can then enter a new code.

Example:

`5678 #`

The system asks the user to confirm it:

`5678 #`

If both entries match, the OLED displays:

`NEW CODE SAVED!`

---

## Important Password Note

The current version stores the changed password in RAM.

This means that if the ESP32 is restarted or loses power, the password returns to the default value:

`1234`

A future version will use ESP32 Preferences/NVS memory so the password remains saved after power is removed.

---

## Hardware Prototype Demonstration

The following images show the ESP32 access-control system operating on the physical breadboard prototype.

### 1. Waiting for User Code
<img width="906" height="595" alt="image" src="https://github.com/user-attachments/assets/02f8ee8b-1308-4c5b-9015-344ee2991081" />




The system prompts the user to enter an access code using the 4x4 keypad.


### 2. Access Granted

When the correct code is entered, the OLED displays **ACCESS GRANTED** and the buzzer provides confirmation feedback.
<img width="898" height="669" alt="image" src="https://github.com/user-attachments/assets/8e02015d-b6f7-4c53-b742-d176890e4c65" />



### 3. Access Denied

When an incorrect code is entered, the OLED displays **ACCESS DENIED** and the buzzer provides an error indication.

<img width="904" height="677" alt="image" src="https://github.com/user-attachments/assets/ef8c8952-dcac-4aa8-a2fa-48b005a9c57d" />

---

## Firmware

The main ESP32 firmware is located at:

`src/esp32_access_control.ino`

The firmware handles:

- Keypad scanning
- User input
- Password verification
- OLED control
- Buzzer control
- Access Granted state
- Access Denied state
- Password changes
- State-machine operation
- Serial debugging

---

## Arduino Libraries

The project uses the following libraries:

```cpp
#include <Wire.h>
#include <Keypad.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
```
## Progress Update – September 22, 2026

### Board 2 – Machine Control & Safety Module

Today I completed and tested the second ESP32 board for my Secure Machine Control System.

### Features Implemented

- 28BYJ-48 5V stepper motor control
- Stepper motor driver interface
- 5V cooling fan controlled using an N-channel MOSFET
- Emergency-stop push button
- Latched emergency-stop logic
- SSD1306 OLED status display
- Green system-status LED
- Red emergency-status LED

### Pin Assignment

| Component | ESP32 GPIO |
|---|---|
| Stepper IN1 | GPIO 16 |
| Stepper IN2 | GPIO 17 |
| Stepper IN3 | GPIO 18 |
| Stepper IN4 | GPIO 19 |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| Fan MOSFET Gate | GPIO 25 |
| Emergency Stop | GPIO 27 |
| Green LED | GPIO 32 |
| Red LED | GPIO 33 |

## Normal Operating Condition

During normal operation:

- Stepper motor is running
- Cooling fan is ON
- Green LED is ON
- Red LED is OFF
- Emergency stop is inactive
- OLED displays the system status

### Normal Condition Photo

<img width="1600" height="1200" alt="image" src="https://github.com/user-attachments/assets/3f7ba72c-844e-4792-9a79-6a7e0b315c4c" />


OLED display:

SYSTEM RUNNING  
MOTOR: ON  
FAN: ON  
E-STOP: OK  

## Emergency Stop Condition

When the emergency-stop button is pressed:

- Stepper motor stops
- Cooling fan turns OFF
- Green LED turns OFF
- Red LED turns ON
- OLED displays an emergency warning
- The emergency condition remains latched after the button is released

### Emergency Stop Photo
<img width="1600" height="1200" alt="image" src="https://github.com/user-attachments/assets/ec63fd6c-6e3f-44d3-8fb9-e676dda87781" />


OLED display:

EMERGENCY STOP  
MOTOR: OFF  
FAN: OFF  
SYSTEM LOCKED  

## Fan Control

The cooling fan is controlled using an N-channel MOSFET. GPIO 25 controls the MOSFET gate.

A series gate resistor is used between the ESP32 and the MOSFET, while a 10kΩ pull-down resistor keeps the MOSFET OFF when the GPIO is floating.

## Next Steps

- Connect Board 1 and Board 2 using ESP-NOW
- Add keypad authentication
- Add normal START and STOP commands
- Send machine status from Board 2 back to Board 1
- Improve the emergency-stop system with hardware-level shutdown
