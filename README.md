# ESP32 Secure Motor Control System

An ESP32-based access control and motor control project that uses a 4x4 keypad, OLED display, and buzzer to authenticate a user before allowing equipment to operate.

The project is being developed in stages, beginning with the security and authentication system and later expanding to motor/fan control, timer operation, emergency-stop protection, and PCB implementation.

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
- Hidden password entry using `*`
- Access Granted notification
- Access Denied notification
- Ability to change the access code
- Current-code verification before changing password
- Serial Monitor debugging

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
| Resistor(s) | As required | Circuit protection |

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
3. The OLED displays:

   `ENTER CODE`

4. The user enters an access code using the keypad.
5. Each entered digit is hidden using a `*`.
6. The user presses `#` to submit the code.
7. The ESP32 compares the entered code with the stored code.
8. If the code is correct:
   - OLED displays `ACCESS GRANTED`
   - Buzzer produces a confirmation sound
9. If the code is incorrect:
   - OLED displays `ACCESS DENIED`
   - Buzzer produces an error sound
10. The user can press `A` to begin the password-change process.

---

## Keypad Controls

| Key | Function |
|---|---|
| `0-9` | Enter digits |
| `#` | Enter / Submit |
| `*` | Clear or Cancel |
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
