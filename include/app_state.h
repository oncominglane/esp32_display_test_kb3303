#pragma once

#include <array>
#include <cstdint>

constexpr std::uint8_t PARAMETER_COUNT = 10;
constexpr std::uint8_t VISIBLE_PARAMETER_COUNT = 4;

enum class ParameterColor : std::uint8_t {
  Green = 0,
  Red = 1,
};

struct ParameterItem {
  const char* name;
  ParameterColor color;
};

struct AppState {
  std::array<ParameterItem, PARAMETER_COUNT> parameters;
  std::uint8_t selectedIndex;
  std::uint8_t scrollOffset;
};

inline AppState createDefaultAppState() {
  return AppState{
      std::array<ParameterItem, PARAMETER_COUNT>{{
          {"Parameter 1", ParameterColor::Green},
          {"Parameter 2", ParameterColor::Green},
          {"Parameter 3", ParameterColor::Green},
          {"Parameter 4", ParameterColor::Green},
          {"Parameter 5", ParameterColor::Green},
          {"Parameter 6", ParameterColor::Green},
          {"Parameter 7", ParameterColor::Green},
          {"Parameter 8", ParameterColor::Green},
          {"Parameter 9", ParameterColor::Green},
          {"Parameter 10", ParameterColor::Green},
      }},
      0,
      0};
}

inline std::uint8_t maxScrollOffset() {
  static_assert(PARAMETER_COUNT >= VISIBLE_PARAMETER_COUNT,
                "Parameter list must contain at least one visible page.");

  return PARAMETER_COUNT - VISIBLE_PARAMETER_COUNT;
}

inline void clampSelection(AppState& state) {
  if (state.selectedIndex >= PARAMETER_COUNT) {
    state.selectedIndex = PARAMETER_COUNT - 1;
  }

  if (state.scrollOffset > maxScrollOffset()) {
    state.scrollOffset = maxScrollOffset();
  }
}

inline void syncScrollToSelection(AppState& state) {
  clampSelection(state);

  if (state.selectedIndex < state.scrollOffset) {
    state.scrollOffset = state.selectedIndex;
    return;
  }

  const std::uint8_t lastVisibleIndex =
      state.scrollOffset + VISIBLE_PARAMETER_COUNT - 1;

  if (state.selectedIndex > lastVisibleIndex) {
    state.scrollOffset = state.selectedIndex - VISIBLE_PARAMETER_COUNT + 1;
  }
}

inline bool moveSelectionUp(AppState& state) {
  if (state.selectedIndex == 0) {
    return false;
  }

  --state.selectedIndex;
  syncScrollToSelection(state);
  return true;
}

inline bool moveSelectionDown(AppState& state) {
  if (state.selectedIndex >= PARAMETER_COUNT - 1) {
    return false;
  }

  ++state.selectedIndex;
  syncScrollToSelection(state);
  return true;
}

inline bool setSelectedParameterColor(AppState& state, ParameterColor color) {
  if (state.selectedIndex >= PARAMETER_COUNT) {
    return false;
  }

  if (state.parameters[state.selectedIndex].color == color) {
    return false;
  }

  state.parameters[state.selectedIndex].color = color;
  return true;
}
