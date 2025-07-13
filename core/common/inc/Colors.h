#ifndef COLORS_H
#define COLORS_H

#include <algorithm>
#include <cstdint>


enum class Colors : uint32_t
{
    TRANSPARENT = 0x00000000U,  // ARGB = (0, 0, 0, 0)
    BLACK       = 0xFF000000U,  // ARGB = (255, 0, 0, 0)
    WHITE       = 0xFFFFFFFFU,  // ARGB = (255, 255, 255, 255)
    RED         = 0xFFFF0000U,  // ARGB = (255, 255, 0, 0)
    GREEN       = 0xFF00FF00U,  // ARGB = (255, 0, 255, 0)
    BLUE        = 0xFF0000FFU,  // ARGB = (255, 0, 0, 255)
    YELLOW      = 0xFFFFFF00U,  // ARGB = (255, 255, 255, 0)
    CYAN        = 0xFF00FFFFU,  // ARGB = (255, 0, 255, 255)
    MAGENTA     = 0xFFFF00FFU,  // ARGB = (255, 255, 0, 255)
    LIGHT_GRAY  = 0xFFD3D3D3U,  // ARGB = (255, 211, 211, 211)
    GRAY        = 0xFF808080U,  // ARGB = (255, 128, 128, 128)
    DARK_GRAY   = 0xFF505050U,  // ARGB = (255, 80, 80, 80)
    PASTEL_PINK = 0xFFFFC0CBU,  // ARGB = (255, 255, 192, 203)
    PASTEL_BLUE = 0xFFAEC6CFU,  // ARGB = (255, 174, 198, 207)
    PASTEL_YELLOW = 0xFFFFF7B2U, // ARGB = (255, 255, 247, 178)
    PASTEL_GREEN = 0xFF77DD77U  // ARGB = (255, 119, 221, 119)
};

// Modern C++23 approach using constexpr to convert enum to uint32_t
constexpr uint32_t toColorValue(Colors color)
{
    return static_cast<uint32_t>(color);
}

constexpr uint32_t applyIntensityToColor(const uint32_t originalColor, float percentageIntensity)
{
    percentageIntensity = std::clamp(percentageIntensity, 0.0f, 1.0f);

    constexpr uint32_t ALPHA_SHIFT = 24;
    constexpr uint32_t RED_SHIFT = 16;
    constexpr uint32_t GREEN_SHIFT = 8;
    constexpr uint32_t COLOR_MASK = 0xFF;

    uint32_t a = (originalColor >> ALPHA_SHIFT) & COLOR_MASK;
    uint32_t r = (originalColor >> RED_SHIFT) & COLOR_MASK;
    uint32_t g = (originalColor >> GREEN_SHIFT) & COLOR_MASK;
    uint32_t b = originalColor & COLOR_MASK;

    r = static_cast<uint32_t>(r * percentageIntensity);
    g = static_cast<uint32_t>(g * percentageIntensity);
    b = static_cast<uint32_t>(b * percentageIntensity);

    return {(a << 24) | (r << 16) | (g << 8) | b};
}
#endif //COLORS_H
