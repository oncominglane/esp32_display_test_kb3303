#include "input_buttons.h"

#include "config.h"

#if defined(ENV_ESP32)
#include <Arduino.h>
#endif

void InputButtons::begin() {
  up_ = makeButtonState(Config::Buttons::UP_PIN);
  down_ = makeButtonState(Config::Buttons::DOWN_PIN);
  left_ = makeButtonState(Config::Buttons::LEFT_PIN);
  right_ = makeButtonState(Config::Buttons::RIGHT_PIN);

#if defined(ENV_ESP32)
  const int mode =
      Config::Buttons::USE_INTERNAL_PULLUP ? INPUT_PULLUP : INPUT;

  if (up_.pin >= 0) {
    pinMode(up_.pin, mode);
  }
  if (down_.pin >= 0) {
    pinMode(down_.pin, mode);
  }
  if (left_.pin >= 0) {
    pinMode(left_.pin, mode);
  }
  if (right_.pin >= 0) {
    pinMode(right_.pin, mode);
  }
#endif
}

ButtonEvent InputButtons::poll() {
#if defined(ENV_ESP32)
  const std::uint32_t nowMs = millis();
#else
  const std::uint32_t nowMs = 0;
#endif

  if (updateButton(up_, nowMs)) {
    return ButtonEvent::Up;
  }

  if (updateButton(down_, nowMs)) {
    return ButtonEvent::Down;
  }

  if (updateButton(left_, nowMs)) {
    return ButtonEvent::Left;
  }

  if (updateButton(right_, nowMs)) {
    return ButtonEvent::Right;
  }

  return ButtonEvent::None;
}

InputButtons::ButtonState InputButtons::makeButtonState(int pin) {
  const bool pressed = readPressed(pin);
  return {pin, pressed, pressed, 0};
}

bool InputButtons::readPressed(int pin) {
  if (pin < 0) {
    return false;
  }

#if defined(ENV_ESP32)
  return digitalRead(pin) == Config::Buttons::PRESSED_LEVEL;
#else
  return false;
#endif
}

bool InputButtons::updateButton(ButtonState& button, std::uint32_t nowMs) {
  if (button.pin < 0) {
    return false;
  }

  const bool rawPressed = readPressed(button.pin);

  if (rawPressed != button.lastRawPressed) {
    button.lastRawPressed = rawPressed;
    button.lastChangeMs = nowMs;
    return false;
  }

  const std::uint32_t elapsedMs = nowMs - button.lastChangeMs;
  if (elapsedMs < Config::Timing::BUTTON_DEBOUNCE_MS) {
    return false;
  }

  if (rawPressed == button.stablePressed) {
    return false;
  }

  button.stablePressed = rawPressed;
  return button.stablePressed;
}
