#include "app_state.h"
#include "display_view.h"
#include "input_buttons.h"
#include "serial_view.h"
#include "storage.h"

#if defined(ENV_ESP32)
#include <Arduino.h>
#else
#include <iostream>
#endif

namespace {

AppState appState = createDefaultAppState();
DisplayView displayView;
InputButtons inputButtons;
SerialView serialView;
Storage storage;

void renderAll() {
  displayView.renderMainScreen(appState);
  serialView.renderMainScreen(appState);
}

bool handleButtonEvent(ButtonEvent event) {
  switch (event) {
    case ButtonEvent::Up:
      return moveSelectionUp(appState);

    case ButtonEvent::Down:
      return moveSelectionDown(appState);

    case ButtonEvent::Left:
      if (setSelectedParameterColor(appState, ParameterColor::Green)) {
        storage.saveParameterColors(appState);
        return true;
      }
      return false;

    case ButtonEvent::Right:
      if (setSelectedParameterColor(appState, ParameterColor::Red)) {
        storage.saveParameterColors(appState);
        return true;
      }
      return false;

    case ButtonEvent::None:
      return false;
  }

  return false;
}

void initializeApplication() {
  serialView.begin();
  displayView.begin();
  inputButtons.begin();

  if (storage.begin()) {
    storage.loadParameterColors(appState);
  }

  syncScrollToSelection(appState);
  displayView.showSplash();
  renderAll();
}

}  // namespace

#if defined(ENV_ESP32)

void setup() {
  initializeApplication();
}

void loop() {
  const ButtonEvent event = inputButtons.poll();

  if (handleButtonEvent(event)) {
    renderAll();
  }

  delay(5);
}

#else

int main() {
  initializeApplication();

  const ButtonEvent demoEvents[] = {
      ButtonEvent::Down,  ButtonEvent::Down, ButtonEvent::Right,
      ButtonEvent::Down,  ButtonEvent::Down, ButtonEvent::Down,
      ButtonEvent::Left,  ButtonEvent::Up,
  };

  for (const ButtonEvent event : demoEvents) {
    if (handleButtonEvent(event)) {
      renderAll();
    }
  }

  storage.end();
  std::cout << "\nNative logic demo finished.\n";
  return 0;
}

#endif
