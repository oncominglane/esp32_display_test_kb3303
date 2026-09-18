# Requirements

## Development Tools

- Visual Studio Code
- PlatformIO extension for Visual Studio Code
- PlatformIO Core 6.x
- Python 3.11 or compatible PlatformIO-supported Python version
- PowerShell on Windows

## PlatformIO Environments

### Native

Used for PC-side testing without ESP32 hardware.

Required:

- PlatformIO `native` platform
- C++17-compatible compiler

On Windows, the current project was checked with:

```text
g++ from Strawberry Perl toolchain
```

PlatformIO may use another compatible compiler depending on the local setup.

### ESP32

Used for firmware compilation.

Required PlatformIO configuration:

```ini
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
```

Required PlatformIO packages are installed automatically by PlatformIO:

- `framework-arduinoespressif32`
- `toolchain-xtensa-esp32s3`
- `toolchain-riscv32-esp`
- `tool-esptoolpy`
- `tool-mklittlefs`

## Arduino Libraries

Declared in `platformio.ini`:

```ini
lib_deps =
  bodmer/TFT_eSPI
```

Framework-provided libraries used by the firmware:

- `LittleFS`
- `Preferences`

## Runtime Assets

The file system image requires:

```text
data/splash.bmp
```

The current BMP loader expects an uncompressed BMP file:

- 16-bit BMP, or
- 24-bit BMP.

## Hardware

Target hardware from the task:

- ESP32-S3-ETH board
- 1.5-inch RGB OLED display module
- four external buttons:
  - Up
  - Down
  - Left
  - Right

Hardware-specific values still need to be assigned before real-device testing:

- button GPIO pins in `include/config.h`;
- display driver and pin configuration for `TFT_eSPI`;
- exact upload/monitor COM port.
