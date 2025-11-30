#include "../Header/TemperatureUI.h"

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace
{
    constexpr float kDigitWidth = 18.0f;
    constexpr float kDigitHeight = 32.0f;
    constexpr float kDigitThickness = 3.0f;
    constexpr float kDigitSpacing = 6.0f;

    // Segment order: A, B, C, D, E, F, G
    const uint8_t kDigitMasks[10] = {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111  // 9
    };

    int clampInt(int v, int lo, int hi)
    {
        if (v < lo) return lo;
        if (v > hi) return hi;
        return v;
    }

    void drawDigit(Renderer2D& renderer, float x, float y, int digit, const Color& color)
    {
        if (digit < 0 || digit > 9) return;

        struct Segment { float x, y, w, h; };
        static const Segment segments[7] = {
            { kDigitThickness, 0.0f, kDigitWidth - 2.0f * kDigitThickness, kDigitThickness }, // A
            { kDigitWidth - kDigitThickness, kDigitThickness, kDigitThickness, kDigitHeight * 0.5f - kDigitThickness }, // B
            { kDigitWidth - kDigitThickness, kDigitHeight * 0.5f, kDigitThickness, kDigitHeight * 0.5f - kDigitThickness }, // C
            { kDigitThickness, kDigitHeight - kDigitThickness, kDigitWidth - 2.0f * kDigitThickness, kDigitThickness }, // D
            { 0.0f, kDigitHeight * 0.5f, kDigitThickness, kDigitHeight * 0.5f - kDigitThickness }, // E
            { 0.0f, kDigitThickness, kDigitThickness, kDigitHeight * 0.5f - kDigitThickness }, // F
            { kDigitThickness, kDigitHeight * 0.5f - kDigitThickness * 0.5f, kDigitWidth - 2.0f * kDigitThickness, kDigitThickness } // G
        };

        uint8_t mask = kDigitMasks[digit];
        for (int i = 0; i < 7; ++i)
        {
            if (mask & (1 << i))
            {
                const auto& s = segments[i];
                renderer.drawRect(x + s.x, y + s.y, s.w, s.h, color);
            }
        }
    }

    void drawMinus(Renderer2D& renderer, float x, float y, const Color& color)
    {
        float lineY = y + (kDigitHeight * 0.5f - kDigitThickness * 0.5f);
        renderer.drawRect(x + kDigitThickness, lineY, kDigitWidth - 2.0f * kDigitThickness, kDigitThickness, color);
    }

    void drawHeatIcon(Renderer2D& renderer, float cx, float cy, float radius, const Color& outer, const Color& inner)
    {
        renderer.drawCircle(cx, cy + radius * 0.25f, radius, outer);
        renderer.drawCircle(cx, cy, radius * 0.65f, inner);
        renderer.drawRect(cx - radius * 0.25f, cy + radius * 0.4f, radius * 0.5f, radius * 0.5f, outer);
    }

    void drawSnowIcon(Renderer2D& renderer, float cx, float cy, float size, const Color& color)
    {
        float arm = size * 0.45f;
        float thickness = size * 0.12f;
        renderer.drawRect(cx - thickness * 0.5f, cy - arm, thickness, arm * 2.0f, color);
        renderer.drawRect(cx - arm, cy - thickness * 0.5f, arm * 2.0f, thickness, color);

        float offset = arm * 0.7f;
        float small = thickness;
        renderer.drawRect(cx - offset - small, cy - small, small, small * 2.0f, color);
        renderer.drawRect(cx + offset, cy - small, small, small * 2.0f, color);
        renderer.drawRect(cx - small, cy - offset - small, small * 2.0f, small, color);
        renderer.drawRect(cx - small, cy + offset, small * 2.0f, small, color);
    }

    void drawCheckIcon(Renderer2D& renderer, float cx, float cy, float size, const Color& color)
    {
        float dot = size * 0.1f;
        float startX = cx - size * 0.35f;
        float startY = cy + size * 0.05f;

        for (int i = 0; i < 4; ++i)
        {
            float step = static_cast<float>(i) * dot * 1.1f;
            renderer.drawRect(startX + step, startY + step, dot, dot, color);
        }

        float midX = startX + 3.0f * dot * 1.1f;
        float midY = startY + 3.0f * dot * 1.1f;
        for (int i = 0; i < 6; ++i)
        {
            float step = static_cast<float>(i) * dot * 1.1f;
            renderer.drawRect(midX + step, midY - step, dot, dot, color);
        }
    }
}

void drawTemperatureValue(Renderer2D& renderer, float value, const RectShape& screen, const Color& color)
{
    int rounded = static_cast<int>(std::round(value));
    rounded = clampInt(rounded, -99, 99);
    bool negative = rounded < 0;
    int absVal = std::abs(rounded);
    int tens = absVal / 10;
    int ones = absVal % 10;
    bool hasTens = absVal >= 10;

    int digitCount = hasTens ? 2 : 1;
    float totalWidth = digitCount * kDigitWidth + (digitCount - 1) * kDigitSpacing;
    if (negative)
    {
        totalWidth += kDigitWidth + kDigitSpacing;
    }

    float startX = screen.x + (screen.w - totalWidth) * 0.5f;
    float startY = screen.y + (screen.h - kDigitHeight) * 0.5f;
    float cursorX = startX;

    if (negative)
    {
        drawMinus(renderer, cursorX, startY, color);
        cursorX += kDigitWidth + kDigitSpacing;
    }

    if (hasTens)
    {
        drawDigit(renderer, cursorX, startY, tens, color);
        cursorX += kDigitWidth + kDigitSpacing;
    }

    drawDigit(renderer, cursorX, startY, ones, color);
}

void drawStatusIcon(Renderer2D& renderer, const RectShape& screen, float desired, float current)
{
    float cx = screen.x + screen.w * 0.5f;
    float cy = screen.y + screen.h * 0.5f;
    float size = std::min(screen.w, screen.h) * 0.35f;

    const float tolerance = 0.25f;
    float diff = desired - current;

    Color heatOuter{ 0.96f, 0.46f, 0.28f, 1.0f };
    Color heatInner{ 0.99f, 0.66f, 0.32f, 1.0f };
    Color snowColor{ 0.66f, 0.85f, 0.98f, 1.0f };
    Color checkColor{ 0.38f, 0.92f, 0.58f, 1.0f };

    if (diff > tolerance)
    {
        drawHeatIcon(renderer, cx, cy, size, heatOuter, heatInner);
    }
    else if (diff < -tolerance)
    {
        drawSnowIcon(renderer, cx, cy, size, snowColor);
    }
    else
    {
        drawCheckIcon(renderer, cx, cy, size, checkColor);
    }
}
