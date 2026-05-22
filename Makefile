# Simple Makefile for WiperPumpControl
# Uses arduino-cli instead of arduino-mk
# Works on macOS and Linux
#
# Prerequisites:
#   Install arduino-cli:
#   macOS: brew install arduino-cli
#   Linux: curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
#
# First-time setup:
#   make setup
#
# Usage:
#   make           - Compile the project
#   make upload    - Upload to Arduino Nano
#   make clean     - Clean build files
#   make monitor   - Open serial monitor
#   make setup     - Install required libraries

# Project Configuration
PROJECT_NAME = WiperPumpControl
BOARD_FQBN   = arduino:avr:nano:cpu=atmega328old
BAUD_RATE    = 9600

# Detect Operating System
UNAME_S := $(shell uname -s)

# Platform-specific port detection
ifeq ($(UNAME_S),Darwin)
    # macOS
    PORT ?= $(shell ls /dev/cu.usbserial* 2>/dev/null | head -n 1)
    ifeq ($(PORT),)
        PORT = /dev/cu.usbserial-*
    endif
else
    # Linux
    PORT ?= $(shell ls /dev/ttyUSB* 2>/dev/null | head -n 1)
    ifeq ($(PORT),)
        PORT = /dev/ttyUSB0
    endif
endif

# Arduino CLI command
ARDUINO_CLI = arduino-cli

# Build directory
BUILD_DIR = build

# HEX files
HEX_FILE = $(BUILD_DIR)/$(PROJECT_NAME).ino.hex
HEX_WITH_BOOTLOADER = $(BUILD_DIR)/$(PROJECT_NAME).ino.with_bootloader.hex
OUTPUT_HEX = $(PROJECT_NAME).hex

# Targets
.PHONY: all compile upload clean monitor setup help info

all: compile

compile:
	@echo "Compiling $(PROJECT_NAME)..."
	$(ARDUINO_CLI) compile --fqbn $(BOARD_FQBN) --build-path $(BUILD_DIR) .
	@echo ""
	@echo "✓ Build complete!"
	@echo ""
	@echo "Generated files:"
	@echo "  $(HEX_FILE)"
	@echo "    └─ Application code (22KB) - for uploading"
	@echo "  $(HEX_WITH_BOOTLOADER)"
	@echo "    └─ App + Bootloader (27KB) - for fresh chips"
	@echo ""
	@echo "Copying HEX to root directory..."
	@cp $(HEX_FILE) ./$(OUTPUT_HEX)
	@echo "✓ $(OUTPUT_HEX) ready for distribution"
	@echo ""

upload: compile
	@echo "Uploading to $(PORT)..."
	$(ARDUINO_CLI) upload -p $(PORT) --fqbn $(BOARD_FQBN) --input-dir $(BUILD_DIR) .

clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILD_DIR)/
	@rm -f $(OUTPUT_HEX)

monitor:
	@echo "Opening serial monitor on $(PORT) at $(BAUD_RATE) baud..."
	@echo "Press Ctrl+C to exit"
	$(ARDUINO_CLI) monitor -p $(PORT) -c baudrate=$(BAUD_RATE)

setup:
	@echo "Setting up Arduino CLI..."
	$(ARDUINO_CLI) core update-index
	$(ARDUINO_CLI) core install arduino:avr
	@echo "Installing required libraries..."
	$(ARDUINO_CLI) lib install "LiquidCrystal I2C"
	@echo "Setup complete!"

info:
	@echo "Project Configuration:"
	@echo "====================="
	@echo "Project: $(PROJECT_NAME)"
	@echo "Board: $(BOARD_FQBN)"
	@echo "Port: $(PORT)"
	@echo "Baud Rate: $(BAUD_RATE)"
	@echo "OS: $(UNAME_S)"
	@echo ""
	@echo "Arduino CLI version:"
	@$(ARDUINO_CLI) version

help:
	@echo "WiperPumpControl Makefile (arduino-cli)"
	@echo "========================================"
	@echo ""
	@echo "Available targets:"
	@echo "  make              - Compile the project"
	@echo "  make upload       - Upload to Arduino Nano"
	@echo "  make clean        - Clean build files"
	@echo "  make monitor      - Open serial monitor"
	@echo "  make setup        - Install libraries (first time)"
	@echo "  make info         - Show configuration"
	@echo "  make export-hex   - Export HEX to current directory"
	@echo "  make help         - Show this help"
	@echo ""
	@echo "Build files: build/"
	@echo "  ├─ .ino.hex              (application only)"
	@echo "  └─ .ino.with_bootloader.hex (app + bootloader)"
	@echo ""
	@echo "First time setup:"
	@echo "  1. brew install arduino-cli"
	@echo "  2. make setup"
	@echo "  3. make upload"