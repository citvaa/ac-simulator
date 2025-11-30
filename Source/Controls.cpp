#include "../Header/Controls.h"

bool pointInRect(double px, double py, const RectShape& rect)
{
    return px >= rect.x && px <= rect.x + rect.w && py >= rect.y && py <= rect.y + rect.h;
}

void drawHalfArrow(Renderer2D& renderer, const RectShape& button, bool isUp, const Color& arrowColor, const Color& bgColor)
{
    float cx = button.x + button.w * 0.5f;
    float cy = button.y + button.h * 0.5f;
    float margin = button.w * 0.22f;
    float topY = button.y + margin;
    float bottomY = button.y + button.h - margin;

    renderer.drawRect(button.x, button.y, button.w, button.h, bgColor);

    float ax = cx;
    float bx = button.x + margin;
    float cxv = button.x + button.w - margin;

    if (isUp)
    {
        renderer.drawTriangle(ax, topY, bx, bottomY, cxv, bottomY, arrowColor);
    }
    else
    {
        renderer.drawTriangle(ax, bottomY, bx, topY, cxv, topY, arrowColor);
    }
}
