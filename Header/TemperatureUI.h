#pragma once

#include "../Header/Renderer2D.h"

void drawTemperatureValue(Renderer2D& renderer, float value, const RectShape& screen, const Color& color);
void drawStatusIcon(Renderer2D& renderer, const RectShape& screen, float desired, float current);
