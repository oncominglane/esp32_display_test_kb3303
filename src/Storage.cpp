#include "storage.h"

#if defined(ENV_ESP32)
#include <Preferences.h>
#endif

namespace {

constexpr const char* STORAGE_NAMESPACE = "params";
constexpr const char* COLORS_KEY = "colors";

#if defined(ENV_ESP32)
Preferences preferences;
#endif

std::uint16_t packColors(const AppState& state) {
  std::uint16_t mask = 0;

  for (std::uint8_t i = 0; i < PARAMETER_COUNT; ++i) {
    if (state.parameters[i].color == ParameterColor::Red) {
      mask |= static_cast<std::uint16_t>(1U << i);
    }
  }

  return mask;
}

void unpackColors(AppState& state, std::uint16_t mask) {
  for (std::uint8_t i = 0; i < PARAMETER_COUNT; ++i) {
    const bool isRed = (mask & static_cast<std::uint16_t>(1U << i)) != 0;
    state.parameters[i].color =
        isRed ? ParameterColor::Red : ParameterColor::Green;
  }
}

}  // namespace

bool Storage::begin() {
#if defined(ENV_ESP32)
  return preferences.begin(STORAGE_NAMESPACE, false);
#else
  return true;
#endif
}

void Storage::end() {
#if defined(ENV_ESP32)
  preferences.end();
#endif
}

bool Storage::loadParameterColors(AppState& state) {
#if defined(ENV_ESP32)
  if (!preferences.isKey(COLORS_KEY)) {
    return false;
  }

  const std::uint16_t colorsMask = preferences.getUShort(COLORS_KEY, 0);
  unpackColors(state, colorsMask);
  return true;
#else
  (void)state;
  return false;
#endif
}

bool Storage::saveParameterColors(const AppState& state) {
#if defined(ENV_ESP32)
  const std::uint16_t colorsMask = packColors(state);
  return preferences.putUShort(COLORS_KEY, colorsMask) == sizeof(colorsMask);
#else
  (void)state;
  return true;
#endif
}
