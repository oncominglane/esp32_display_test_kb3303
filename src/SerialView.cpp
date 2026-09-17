#include "SerialView.h"

#include "Config.h"

#if defined(ENV_ESP32)
#include <Arduino.h>
#else
#include <iostream>
#endif

void SerialView::begin() {
#if defined(ENV_ESP32)
  Serial.begin(Config::Serial::BAUD_RATE);
  while (!Serial) {
  }
#endif
}

void SerialView::renderMainScreen(const AppState& state) {
#if defined(ENV_ESP32)
  Serial.print("\033[2J\033[H");
#else
  std::cout << "\033[2J\033[H";
#endif

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
    std::cout << selectionMarker(selected) << " ["
              << colorName(parameter.color) << "] " << parameter.name << '\n';
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

char SerialView::selectionMarker(bool selected) {
  return selected ? '>' : ' ';
}
