#pragma once

#include "AppState.h"

class Storage {
 public:
  bool begin();
  void end();

  bool loadParameterColors(AppState& state);
  bool saveParameterColors(const AppState& state);
};
