# ESP32-S3 Display Test

PlatformIO/Arduino project for the ESP32-S3-ETH test task.

The firmware shows a splash image from the ESP32 file system, then displays a
scrollable list of 10 parameters. Four parameters are visible at a time. The
selected parameter is highlighted, and its color can be changed with left/right
controls.

## Project Structure

```text
data/
  splash.bmp

docs/
  AI_PROMPTS.md
  images/
  task_summary.md

include/
  app_state.h
  config.h
  display_view.h
  input_buttons.h
  serial_view.h
  storage.h

src/
  display_view.cpp
  input_buttons.cpp
  main.cpp
  serial_view.cpp
  storage.cpp

platformio.ini
README.md
requirements.md
```

## Environments

The project has two PlatformIO environments:

```ini
[env:native]
```

Builds and runs the application logic on a PC without ESP32 hardware.

```ini
[env:esp32]
```

Builds the firmware for ESP32-S3 using the Arduino framework.

## Native Build and Test

Use this mode when the ESP32 board is not available.

Build:

```powershell
pio run -e native
```

Run:

```powershell
.pio\build\native\program.exe
```

Controls:

```text
W - move selection up
S - move selection down
A - set selected parameter to green
D - set selected parameter to red
Q - quit
```

The native version redraws the console in place. The selected row is shown with
a yellow background, and parameter text is shown in green or red.

## ESP32 Build

Build firmware:

```powershell
pio run -e esp32
```

Build the LittleFS image from the `data/` directory:

```powershell
pio run -e esp32 -t buildfs
```

Upload firmware when hardware is available:

```powershell
pio run -e esp32 -t upload
```

Upload the file system when hardware is available:

```powershell
pio run -e esp32 -t uploadfs
```

Open serial monitor:

```powershell
pio device monitor -e esp32
```

## Splash Image

The splash image is stored at:

```text
data/splash.bmp
```

In firmware it is opened as:

```text
/splash.bmp
```

The current BMP loader supports uncompressed 16-bit and 24-bit BMP files.

## Hardware Configuration

Button GPIOs are configured in:

```text
include/config.h
```

Current placeholders:

```cpp
constexpr int UP_PIN = UNASSIGNED_GPIO;
constexpr int DOWN_PIN = UNASSIGNED_GPIO;
constexpr int LEFT_PIN = UNASSIGNED_GPIO;
constexpr int RIGHT_PIN = UNASSIGNED_GPIO;
```

Replace them with actual GPIO numbers before testing on hardware.

The display layer currently uses `TFT_eSPI`. The exact display driver, bus and
pin configuration must be adjusted for the real 1.5-inch RGB OLED module before
hardware testing.

## Persistent Storage

Parameter colors are saved with ESP32 `Preferences`/NVS.

The storage format is a 16-bit bitmask:

- bit `0` stores `Parameter 1`;
- bit `1` stores `Parameter 2`;
- ...
- bit `9` stores `Parameter 10`;
- `0` means green;
- `1` means red.

## Current Verification Status

Verified without hardware:

- native build;
- native keyboard control;
- list navigation;
- list boundaries;
- scrolling;
- green/red color changes;
- selected-row highlighting;
- ESP32 firmware compilation;
- LittleFS image build.

Not verified without hardware:

- real display initialization;
- real button GPIO input;
- firmware upload;
- LittleFS upload;
- splash rendering on the physical display.
