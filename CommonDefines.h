#ifndef COMMONDEFINES_H
#define COMMONDEFINES_H
#include <array>
#include <cstddef>
#include <cstdint>

constexpr auto TARGETED_FRAME_RATE = 60u;
constexpr auto FRAME_TIME = 1000 / TARGETED_FRAME_RATE;
constexpr size_t WINDOW_WIDTH = 800u;
constexpr size_t WINDOW_HEIGHT = 600u;
constexpr size_t COLOR_BUFFER_SIZE = WINDOW_WIDTH*WINDOW_HEIGHT;
constexpr uint32_t ZERO_VALUE_COLOR_BUFFER = 0xFF000000;
using ColorBufferArray = std::array<uint32_t, COLOR_BUFFER_SIZE>;

#endif //COMMONDEFINES_H
