# Wiper Pump Control System

Arduino-based wiper pump motor controller with EEPROM-based cycle counting, LCD display, and power-loss recovery.

## Features

- ✅ 2000 motor ON/OFF cycles per run
- ✅ Power-loss recovery (resumes from last saved cycle)
- ✅ 20x4 I2C LCD display for status
- ✅ EEPROM wear leveling
- ✅ 5-second hold reset button
- ✅ Serial debugging output
- ✅ Production-ready system

## Hardware Requirements

- **Arduino Nano** (ATmega328P)
- **20x4 I2C LCD** (Address: 0x27)
- **Motor** (connected to Pin 13)
- **Reset Button** (connected to Pin 2 with GND)
- **Power Supply** (appropriate for motor)

## Wiring Diagram

```
Arduino Nano Connections:
├── Pin 13 → Motor Control (via relay/driver)
├── Pin 2  → Reset Button → GND
├── SDA    → LCD SDA
├── SCL    → LCD SCL
└── GND    → LCD GND, Button GND
```

## Software Requirements

### For Arduino IDE (GUI)
- Arduino IDE 1.8.x or 2.x
- LiquidCrystal_I2C library

### For Command Line (Makefile)
- arduino-cli (official Arduino command-line tool)

## Installation

### macOS

```bash
# Install arduino-cli
brew install arduino-cli

# Navigate to project
cd <path>/WiperPumpControl

# First-time setup (installs libraries and board support)
make setup
```

### Linux

```bash
# Install arduino-cli
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

# Add to PATH (add to ~/.bashrc or ~/.zshrc)
export PATH=$PATH:$HOME/bin

# Navigate to project
cd <path>/WiperPumpControl

# First-time setup (installs libraries and board support)
make setup
```

## Compilation

### Using Arduino IDE

1. Open `WiperPumpControl.ino` in Arduino IDE
2. Select **Tools → Board → Arduino Nano**
3. Select **Tools → Processor → ATmega328P (Old Bootloader)**
4. Select **Tools → Port → /dev/cu.usbserial*** (your port)
5. Click **Upload**

### Using Makefile (Command Line)

```bash
# Show available commands
make help

# Compile the project (automatically creates WiperPumpControl.hex in root)
make

# Upload to Arduino Nano
make upload

# Open serial monitor
make monitor

# Clean build files
make clean

# Show configuration
make info
```

## Usage

### Normal Operation

1. **Power On**: System reads EEPROM and resumes from last cycle
2. **LCD Display**: Shows current cycle, remaining, and motor status
3. **Motor Cycles**: 3 seconds ON, 27 seconds OFF (30 sec total)
4. **Completion**: After 2000 cycles, shows "CYCLE COMPLETE!"

### Reset Button

**To Reset During Cycles:**
1. Press and hold reset button
2. LCD shows countdown: "Hold for: 5 sec"
3. Hold for full 5 seconds
4. System clears counter and restarts from cycle 1

**To Restart After Completion:**
1. Press and hold reset button for 5 seconds
2. System moves to next EEPROM location
3. Starts new run from cycle 1

### Serial Monitor

Connect at **9600 baud** to see detailed debug information:
- EEPROM location and count values
- Power-loss recovery detection
- Cycle progress
- Motor status
- EEPROM write verification

## Project Structure

```
WiperPumpControl/
├── WiperPumpControl.ino    # Main program
├── eepromControl.h/cpp     # EEPROM management
├── lcdControl.h/cpp        # LCD display functions
├── buttonControl.h/cpp     # Reset button handling
├── pinDescription.h        # Hardware pin definitions
├── Makefile                # Command-line compilation
├── Doxyfile                # Documentation generation
└── README.md               # This file
```

## Configuration

### Timing (WiperPumpControl.ino)

```cpp
#define NUM_OF_MAX_COUNT 2000   // Total cycles per run
#define MOTOR_ON_TIME    3000   // Motor ON duration (ms)
#define MOTOR_OFF_TIME   27000  // Motor OFF duration (ms)
```

### Hardware Pins (pinDescription.h)

```cpp
#define MOTOR_PIN         13    // Motor control pin
#define RESET_BUTTON_PIN  2     // Reset button pin
#define LCD_MODULE_ADDRESS 0x27 // I2C LCD address
```

## Documentation

Generate HTML documentation using Doxygen:

```bash
# Generate documentation
doxygen Doxyfile

# Open in browser
open docs/html/index.html
```

## Troubleshooting

### Compilation Errors

**Error: LiquidCrystal_I2C not found**
- Install library: Sketch → Include Library → Manage Libraries → Search "LiquidCrystal I2C"

**Error: Port not found**
- Check USB connection
- Update MONITOR_PORT in Makefile
- Check: `ls /dev/cu.*` (macOS) or `ls /dev/ttyUSB*` (Linux)

### Runtime Issues

**LCD not displaying**
- Check I2C address (default: 0x27)
- Verify wiring (SDA, SCL, GND, VCC)
- Run I2C scanner sketch

**Motor not running**
- Check pin 13 connection
- Verify motor driver/relay
- Check power supply

**Reset button not working**
- Verify button wiring (Pin 2 to GND)
- Check button is normally open
- Test with serial monitor

## License

This project is open source. Feel free to modify and use for your needs.

## Author

Developed for wiper pump control applications with industrial-grade reliability.

## Version History

- **v1.0** - Initial release with basic motor control
- **v2.0** - Added LCD display and power-loss recovery
- **v3.0** - Added reset button and modular code structure