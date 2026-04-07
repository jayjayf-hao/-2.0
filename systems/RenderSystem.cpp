#include "systems/RenderSystem.h"
#include <algorithm>

RenderSystem::RenderSystem(int width, int height)
    : bufferWidth(width),
      bufferHeight(height),
      swapFlag(false),
      hBufferA(INVALID_HANDLE_VALUE),
      hBufferB(INVALID_HANDLE_VALUE) {
    frameA.assign(bufferHeight, std::vector<char>(bufferWidth, ' '));
    frameB.assign(bufferHeight, std::vector<char>(bufferWidth, ' '));
    colorA.assign(bufferHeight, std::vector<WORD>(bufferWidth, DefaultColor));
    colorB.assign(bufferHeight, std::vector<WORD>(bufferWidth, DefaultColor));

    hBufferA = CreateConsoleScreenBuffer(
        GENERIC_WRITE,
        FILE_SHARE_WRITE,
        nullptr,
        CONSOLE_TEXTMODE_BUFFER,
        nullptr
    );
    hBufferB = CreateConsoleScreenBuffer(
        GENERIC_WRITE,
        FILE_SHARE_WRITE,
        nullptr,
        CONSOLE_TEXTMODE_BUFFER,
        nullptr
    );

    CONSOLE_CURSOR_INFO cursorInfo = {1, FALSE};
    SetConsoleCursorInfo(hBufferA, &cursorInfo);
    SetConsoleCursorInfo(hBufferB, &cursorInfo);

    SetConsoleActiveScreenBuffer(hBufferA);
}

RenderSystem::~RenderSystem() {
    if (hBufferA != INVALID_HANDLE_VALUE) {
        CloseHandle(hBufferA);
    }
    if (hBufferB != INVALID_HANDLE_VALUE) {
        CloseHandle(hBufferB);
    }
}

void RenderSystem::clear() {
    std::vector<std::vector<char>>& frame = activeFrame();
    std::vector<std::vector<WORD>>& colors = activeColor();
    for (int row = 0; row < bufferHeight; ++row) {
        std::fill(frame[row].begin(), frame[row].end(), ' ');
        std::fill(colors[row].begin(), colors[row].end(), DefaultColor);
    }
}

void RenderSystem::drawChar(int x, int y, char c, WORD color) {
    if (x < 0 || x >= bufferWidth || y < 0 || y >= bufferHeight) {
        return;
    }
    activeFrame()[y][x] = c;
    activeColor()[y][x] = color;
}

void RenderSystem::drawText(int x, int y, const std::string& text, WORD color) {
    for (size_t i = 0; i < text.size(); ++i) {
        drawChar(x + static_cast<int>(i), y, text[i], color);
    }
}

void RenderSystem::present() {
    std::vector<std::vector<char>>& frame = activeFrame();
    std::vector<std::vector<WORD>>& colors = activeColor();
    HANDLE handle = activeHandle();

    COORD cursor = {0, 0};
    DWORD written = 0;
    for (int row = 0; row < bufferHeight; ++row) {
        WriteConsoleOutputCharacterA(handle, frame[row].data(), bufferWidth, cursor, &written);
        WriteConsoleOutputAttribute(handle, colors[row].data(), bufferWidth, cursor, &written);
        cursor.Y++;
    }

    swapFlag = !swapFlag;
    SetConsoleActiveScreenBuffer(activeHandle());
}

HANDLE RenderSystem::activeHandle() const {
    return swapFlag ? hBufferB : hBufferA;
}

std::vector<std::vector<char>>& RenderSystem::activeFrame() {
    return swapFlag ? frameB : frameA;
}

std::vector<std::vector<WORD>>& RenderSystem::activeColor() {
    return swapFlag ? colorB : colorA;
}
