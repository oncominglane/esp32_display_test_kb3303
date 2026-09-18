#include "serial_view.h"

#include "config.h"

#if defined(ENV_ESP32)
#include <Arduino.h>
#else
#include <iostream>
#include <string>
#if defined(_WIN32)
#include <windows.h>
#endif
#endif

namespace {

void clearConsoleScreen() {
#if defined(ENV_ESP32)
  Serial.print("\033[2J\033[H");
#elif defined(_WIN32)
  const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
  if (output == INVALID_HANDLE_VALUE) {
    return;
  }

  CONSOLE_SCREEN_BUFFER_INFO screenInfo{};
  if (!GetConsoleScreenBufferInfo(output, &screenInfo)) {
    return;
  }

  const DWORD cellCount =
      static_cast<DWORD>(screenInfo.dwSize.X) * screenInfo.dwSize.Y;
  const COORD homePosition{0, 0};
  DWORD written = 0;

  FillConsoleOutputCharacterA(output, ' ', cellCount, homePosition, &written);
  FillConsoleOutputAttribute(output, screenInfo.wAttributes, cellCount,
                             homePosition, &written);
  SetConsoleCursorPosition(output, homePosition);
#else
  std::cout << "\033[2J\033[H";
#endif
}

#if !defined(ENV_ESP32)
int consoleWidth() {
#if defined(_WIN32)
  const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
  if (output == INVALID_HANDLE_VALUE) {
    return 80;
  }

  CONSOLE_SCREEN_BUFFER_INFO screenInfo{};
  if (!GetConsoleScreenBufferInfo(output, &screenInfo)) {
    return 80;
  }

  return screenInfo.dwSize.X;
#else
  return 80;
#endif
}
#endif

}  // namespace

void SerialView::begin() {
#if defined(ENV_ESP32)
  Serial.begin(Config::Serial::BAUD_RATE);
  const std::uint32_t startMs = millis();
  while (!Serial && millis() - startMs < 2000) {
    delay(10);
  }
#endif
}

void SerialView::renderMainScreen(const AppState& state) {
  clearConsoleScreen();

  for (std::uint8_t row = 0; row < VISIBLE_PARAMETER_COUNT; ++row) {
    const std::uint8_t parameterIndex = state.scrollOffset + row;
    if (parameterIndex >= PARAMETER_COUNT) {
      break;
    }

    const ParameterItem& parameter = state.parameters[parameterIndex];
    const bool selected = parameterIndex == state.selectedIndex;

#if defined(ENV_ESP32)
    Serial.print(selectionMarker(selected));
    Serial.print(" [");
    Serial.print(colorName(parameter.color));
    Serial.print("] ");
    Serial.println(parameter.name);
#else
    const std::string line =
        std::string(1, selectionMarker(selected)) + " " + parameter.name;

    if (selected) {
      const int paddingSize = consoleWidth() - static_cast<int>(line.size());
      std::cout << "\033[43m\033[30m" << line;
      if (paddingSize > 0) {
        std::cout << std::string(paddingSize, ' ');
      }
      std::cout << "\033[0m\n";
    } else {
      std::cout << line.substr(0, 2) << ansiColor(parameter.color)
                << line.substr(2) << "\033[0m\n";
    }
#endif
  }

#if !defined(ENV_ESP32)
  std::cout.flush();
#endif
}

const char* SerialView::colorName(ParameterColor color) {
  switch (color) {
    case ParameterColor::Green:
      return "GREEN";
    case ParameterColor::Red:
      return "RED";
  }

  return "UNKNOWN";
}

const char* SerialView::ansiColor(ParameterColor color) {
#if !defined(ENV_ESP32)
  switch (color) {
    case ParameterColor::Green:
      return "\033[32m";
    case ParameterColor::Red:
      return "\033[31m";
  }

  return "\033[0m";
#else
  (void)color;
  return "";
#endif
}

char SerialView::selectionMarker(bool selected) {
  return selected ? '>' : ' ';
}
