#pragma once

#include <cstdint>

namespace Config {

namespace Serial {
constexpr std::uint32_t BAUD_RATE = 115200;
}  // namespace Serial

namespace Filesystem {
constexpr const char* SPLASH_PATH = "/splash.bmp";
}  // namespace Filesystem

namespace Timing {
constexpr std::uint32_t SPLASH_DURATION_MS = 10000;
constexpr std::uint32_t BUTTON_DEBOUNCE_MS = 40;
constexpr std::uint32_t SERIAL_REDRAW_DELAY_MS = 10;
}  // namespace Timing

namespace Buttons {
constexpr int UNASSIGNED_GPIO = -1;

constexpr int UP_PIN = UNASSIGNED_GPIO;
constexpr int DOWN_PIN = UNASSIGNED_GPIO;
constexpr int LEFT_PIN = UNASSIGNED_GPIO;
constexpr int RIGHT_PIN = UNASSIGNED_GPIO;

constexpr bool PRESSED_LEVEL = false;
constexpr bool USE_INTERNAL_PULLUP = true;
}  // namespace Buttons

namespace Display {
constexpr std::uint16_t COLOR_BACKGROUND = 0x0000;
constexpr std::uint16_t COLOR_TEXT_GREEN = 0x07E0;
constexpr std::uint16_t COLOR_TEXT_RED = 0xF800;
constexpr std::uint16_t COLOR_SELECTION = 0xFFE0;
constexpr std::uint16_t COLOR_SELECTION_TEXT = 0x0000;

constexpr std::uint8_t TEXT_SIZE = 2;
constexpr std::uint8_t ROW_PADDING_X = 8;
constexpr std::uint8_t ROW_PADDING_Y = 4;
}  // namespace Display

}  // namespace Config
