#pragma once

#include "AppState.h"

class SerialView {
 public:
  void begin();
  void renderMainScreen(const AppState& state);

 private:
  static const char* colorName(ParameterColor color);
  static char selectionMarker(bool selected);
};
