#ifndef COLORS_H
#define COLORS_H
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
#endif //COLORS_H
