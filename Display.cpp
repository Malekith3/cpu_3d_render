#include "Display.h"
#include <span>
#include <bits/ranges_algobase.h>


namespace Render
{

void drawGrid(ColorBufferArray& colorBuffer, uint32_t gridColor, size_t gridSpacing, size_t gridWidth)
{
    for (size_t i{0u}; i < colorBuffer.size(); i++)
    {
        size_t columnIndex = i % WINDOW_WIDTH;
        size_t rowIndex = i / WINDOW_WIDTH;

        if (columnIndex % gridSpacing < gridWidth || rowIndex % gridSpacing < gridWidth)
        {
            colorBuffer[i] = gridColor;
        }
    }
}

void drawRect(ColorBufferArray& colorBuffer, int posX, int posY, size_t width, size_t height, uint32_t color)
{
    size_t numberOfRow = posY;
    while (numberOfRow < posY + height)
    {
        const size_t startIdx = numberOfRow * WINDOW_WIDTH + posX;
        auto row = std::span(colorBuffer.data() + startIdx, width);
        std::ranges::fill(row, color);
        numberOfRow++;
    }
}

void drawPixel(ColorBufferArray& colorBuffer, int posX, int posY, uint32_t color)
{
    size_t index{WINDOW_WIDTH*posY + posX};
    if (index < colorBuffer.size())
    {
        colorBuffer[index] = color;
    }
}

}
