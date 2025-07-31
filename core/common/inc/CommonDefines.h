#ifndef COMMONDEFINES_H
#define COMMONDEFINES_H
#include <array>
#include <vector>
#include <cstddef>
#include <cstdint>

constexpr auto TARGETED_FRAME_RATE = 60u;
constexpr auto FRAME_TIME = 1000 / TARGETED_FRAME_RATE;
constexpr size_t WINDOW_WIDTH = 1920u;
constexpr size_t WINDOW_HEIGHT = 1080u;
constexpr size_t COLOR_BUFFER_SIZE = WINDOW_WIDTH*WINDOW_HEIGHT;
constexpr uint32_t ZERO_VALUE_COLOR_BUFFER = 0x000000FF;
constexpr uint32_t ERROR_COLOR = 0xFFFF00FF;
using ColorBufferArray = std::vector<uint32_t>;

#endif //COMMONDEFINES_H
