#ifndef SNAKEII_RENDERSYSTEM_H
#define SNAKEII_RENDERSYSTEM_H

#include <string>
#include <vector>
#include <windows.h>

class RenderSystem {
public:
    static constexpr WORD DefaultColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    RenderSystem(int width, int height);
    ~RenderSystem();

    void clear();
    void drawChar(int x, int y, char c, WORD color = DefaultColor);
    void drawText(int x, int y, const std::string& text, WORD color = DefaultColor);
    void present();

private:
    int bufferWidth;
    int bufferHeight;
    bool swapFlag;

    HANDLE hBufferA;
    HANDLE hBufferB;

    std::vector<std::vector<char>> frameA;
    std::vector<std::vector<char>> frameB;
    std::vector<std::vector<WORD>> colorA;
    std::vector<std::vector<WORD>> colorB;

    HANDLE activeHandle() const;
    std::vector<std::vector<char>>& activeFrame();
    std::vector<std::vector<WORD>>& activeColor();
};

#endif
