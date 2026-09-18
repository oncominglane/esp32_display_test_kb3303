#include "app_state.h"
#include "display_view.h"
#include "input_buttons.h"
#include "serial_view.h"
#include "storage.h"

#if defined(ENV_ESP32)
#include <Arduino.h>
#else
#include <cctype>
#include <iostream>
#if defined(_WIN32)
#include <conio.h>
#endif
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

namespace {

void printNativeControls() {
  std::cout << "\nW: Up  S: Down  A: Green  D: Red  Q: Quit\n";
}

char readNativeKey() {
#if defined(_WIN32)
  return static_cast<char>(_getch());
#else
  char key = '\0';
  std::cin >> key;
  return key;
#endif
}

ButtonEvent buttonEventFromKey(char key) {
  switch (static_cast<char>(std::tolower(static_cast<unsigned char>(key)))) {
    case 'w':
      return ButtonEvent::Up;
    case 's':
      return ButtonEvent::Down;
    case 'a':
      return ButtonEvent::Left;
    case 'd':
      return ButtonEvent::Right;
    default:
      return ButtonEvent::None;
  }
}

bool isQuitKey(char key) {
  return std::tolower(static_cast<unsigned char>(key)) == 'q';
}

}  // namespace

int main() {
  initializeApplication();
  printNativeControls();

  while (true) {
    const char key = readNativeKey();

    if (isQuitKey(key)) {
      break;
    }

    const ButtonEvent event = buttonEventFromKey(key);
    if (handleButtonEvent(event)) {
      renderAll();
      printNativeControls();
    }
  }

  storage.end();
  std::cout << "\nNative interactive demo finished.\n";
  return 0;
}

#endif
