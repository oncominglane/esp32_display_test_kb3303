#pragma once

#include <cstdint>

enum class ButtonEvent : std::uint8_t {
  None = 0,
  Up,
  Down,
  Left,
  Right,
};

class InputButtons {
 public:
  void begin();
  ButtonEvent poll();

 private:
  struct ButtonState {
    int pin;
    bool stablePressed;
    bool lastRawPressed;
    std::uint32_t lastChangeMs;
  };

  ButtonState up_{};
  ButtonState down_{};
  ButtonState left_{};
  ButtonState right_{};

  static ButtonState makeButtonState(int pin);
  static bool readPressed(int pin);
  static bool updateButton(ButtonState& button, std::uint32_t nowMs);
};
