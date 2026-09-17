#include "display_view.h"

#include "config.h"

#if defined(ENV_ESP32)
#include <Arduino.h>
#include <LittleFS.h>
#include <TFT_eSPI.h>
#endif

namespace {

#if defined(ENV_ESP32)
TFT_eSPI tft;

std::uint16_t read16(File& file) {
  const std::uint16_t low = file.read();
  const std::uint16_t high = file.read();
  return low | (high << 8);
}

std::uint32_t read32(File& file) {
  const std::uint32_t b0 = file.read();
  const std::uint32_t b1 = file.read();
  const std::uint32_t b2 = file.read();
  const std::uint32_t b3 = file.read();
  return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

bool drawBmpFromLittleFs(const char* path, int x, int y) {
  File file = LittleFS.open(path, "r");
  if (!file) {
    return false;
  }

  if (read16(file) != 0x4D42) {
    file.close();
    return false;
  }

  (void)read32(file);
  (void)read32(file);
  const std::uint32_t imageOffset = read32(file);
  const std::uint32_t headerSize = read32(file);

  if (headerSize < 40) {
    file.close();
    return false;
  }

  const std::int32_t bmpWidth = static_cast<std::int32_t>(read32(file));
  const std::int32_t rawHeight = static_cast<std::int32_t>(read32(file));
  const bool topDown = rawHeight < 0;
  const std::int32_t bmpHeight = topDown ? -rawHeight : rawHeight;

  if (bmpWidth <= 0 || bmpHeight <= 0) {
    file.close();
    return false;
  }

  if (read16(file) != 1) {
    file.close();
    return false;
  }

  const std::uint16_t bitsPerPixel = read16(file);
  const std::uint32_t compression = read32(file);

  if (compression != 0 || (bitsPerPixel != 16 && bitsPerPixel != 24)) {
    file.close();
    return false;
  }

  const int drawWidth = min<int>(bmpWidth, tft.width() - x);
  const int drawHeight = min<int>(bmpHeight, tft.height() - y);

  if (drawWidth <= 0 || drawHeight <= 0) {
    file.close();
    return false;
  }

  const std::uint32_t rowSize =
      ((static_cast<std::uint32_t>(bitsPerPixel) * bmpWidth + 31) / 32) * 4;
  std::uint16_t* rowBuffer = new std::uint16_t[drawWidth];

  if (rowBuffer == nullptr) {
    file.close();
    return false;
  }

  for (int row = 0; row < drawHeight; ++row) {
    const int sourceRow = topDown ? row : (bmpHeight - 1 - row);
    const std::uint32_t rowOffset = imageOffset + sourceRow * rowSize;
    file.seek(rowOffset);

    for (int col = 0; col < drawWidth; ++col) {
      if (bitsPerPixel == 24) {
        const std::uint8_t blue = file.read();
        const std::uint8_t green = file.read();
        const std::uint8_t red = file.read();
        rowBuffer[col] = tft.color565(red, green, blue);
      } else {
        rowBuffer[col] = read16(file);
      }
    }

    tft.pushImage(x, y + row, drawWidth, 1, rowBuffer);
  }

  delete[] rowBuffer;
  file.close();
  return true;
}
#endif

}  // namespace

bool DisplayView::begin() {
#if defined(ENV_ESP32)
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(Config::Display::COLOR_BACKGROUND);

  return LittleFS.begin(true);
#else
  return true;
#endif
}

bool DisplayView::showSplash() {
#if defined(ENV_ESP32)
  tft.fillScreen(Config::Display::COLOR_BACKGROUND);

  const bool drawn = drawBmpFromLittleFs(Config::Filesystem::SPLASH_PATH, 0, 0);
  delay(Config::Timing::SPLASH_DURATION_MS);
  return drawn;
#else
  return true;
#endif
}

void DisplayView::renderMainScreen(const AppState& state) {
#if defined(ENV_ESP32)
  tft.fillScreen(Config::Display::COLOR_BACKGROUND);

  const int screenWidth = tft.width();
  const int screenHeight = tft.height();
  const int rowHeight = screenHeight / VISIBLE_PARAMETER_COUNT;

  tft.setTextSize(Config::Display::TEXT_SIZE);
  tft.setTextDatum(ML_DATUM);

  for (std::uint8_t row = 0; row < VISIBLE_PARAMETER_COUNT; ++row) {
    const std::uint8_t parameterIndex = state.scrollOffset + row;
    if (parameterIndex >= PARAMETER_COUNT) {
      break;
    }

    const ParameterItem& parameter = state.parameters[parameterIndex];
    const bool selected = parameterIndex == state.selectedIndex;
    const int rowTop = row * rowHeight;
    const int textY = rowTop + rowHeight / 2;

    if (selected) {
      tft.fillRect(0, rowTop, screenWidth, rowHeight,
                   Config::Display::COLOR_SELECTION);
    }

    const std::uint16_t textColor =
        selected ? Config::Display::COLOR_SELECTION_TEXT
                 : colorForParameter(parameter.color);

    tft.setTextColor(textColor,
                     selected ? Config::Display::COLOR_SELECTION
                              : Config::Display::COLOR_BACKGROUND);
    tft.drawString(parameter.name, Config::Display::ROW_PADDING_X, textY);
  }
#else
  (void)state;
#endif
}

unsigned int DisplayView::colorForParameter(ParameterColor color) {
  switch (color) {
    case ParameterColor::Green:
      return Config::Display::COLOR_TEXT_GREEN;
    case ParameterColor::Red:
      return Config::Display::COLOR_TEXT_RED;
  }

  return Config::Display::COLOR_TEXT_GREEN;
}
