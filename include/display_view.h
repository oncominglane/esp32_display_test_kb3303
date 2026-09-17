#pragma once

#include "app_state.h"

class DisplayView {
 public:
  bool begin();
  bool showSplash();
  void renderMainScreen(const AppState& state);

 private:
  static unsigned int colorForParameter(ParameterColor color);
};
