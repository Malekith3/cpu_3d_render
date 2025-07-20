#ifndef COLORS_H
#define COLORS_H

#include <algorithm>
#include <cstdint>


enum class Colors : uint32_t
{
    TRANSPARENT   = 0x00000000U, // RGBA = (0, 0, 0, 0)
    BLACK         = 0x000000FFU, // RGBA = (0, 0, 0, 255)
    WHITE         = 0xFFFFFFFFU, // RGBA = (255, 255, 255, 255)
    RED           = 0xFF0000FFU, // RGBA = (255, 0, 0, 255)
    GREEN         = 0x00FF00FFU, // RGBA = (0, 255, 0, 255)
    BLUE          = 0x0000FFFFU, // RGBA = (0, 0, 255, 255)
    YELLOW        = 0xFFFF00FFU, // RGBA = (255, 255, 0, 255)
    CYAN          = 0x00FFFFFFU, // RGBA = (0, 255, 255, 255)
    MAGENTA       = 0xFF00FFFFU, // RGBA = (255, 0, 255, 255)
    LIGHT_GRAY    = 0xD3D3D3FFU, // RGBA = (211, 211, 211, 255)
    GRAY          = 0x808080FFU, // RGBA = (128, 128, 128, 255)
    DARK_GRAY     = 0x505050FFU, // RGBA = (80, 80, 80, 255)
    PASTEL_PINK   = 0xFFC0CBFFU, // RGBA = (255, 192, 203, 255)
    PASTEL_BLUE   = 0xAEC6CFFFU, // RGBA = (174, 198, 207, 255)
    PASTEL_YELLOW = 0xFFF7B2FFU, // RGBA = (255, 247, 178, 255)
    PASTEL_GREEN  = 0x77DD77FFU  // RGBA = (119, 221, 119, 255)
};

// Modern C++23 approach using constexpr to convert enum to uint32_t
constexpr uint32_t toColorValue(Colors color)
{
    return static_cast<uint32_t>(color);
}

constexpr uint32_t applyIntensityToColor(const uint32_t originalColor, float percentageIntensity)
{
    percentageIntensity = std::clamp(percentageIntensity, 0.0f, 1.0f);

    constexpr uint32_t BLUE_SHIFT = 8;
    constexpr uint32_t RED_SHIFT = 24;
    constexpr uint32_t GREEN_SHIFT = 16;
    constexpr uint32_t COLOR_MASK = 0xFF;

    uint32_t a = originalColor  & COLOR_MASK;
    uint32_t r = (originalColor >> RED_SHIFT) & COLOR_MASK;
    uint32_t g = (originalColor >> GREEN_SHIFT) & COLOR_MASK;
    uint32_t b = (originalColor >> BLUE_SHIFT) & COLOR_MASK;

    r = static_cast<uint32_t>(r * percentageIntensity);
    g = static_cast<uint32_t>(g * percentageIntensity);
    b = static_cast<uint32_t>(b * percentageIntensity);

    return {(r << 24) | (g << 16) | (b << 8) | a};
}
#endif //COLORS_H
