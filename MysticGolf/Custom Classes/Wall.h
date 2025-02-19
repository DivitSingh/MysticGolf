#ifndef WALL_H
#define WALL_H

#include "app/app.h"

class Wall {
private:
    std::vector<CSimpleSprite*> sprites;
    float x, y;
    float width, height;

public:
    Wall(float startX, float startY, float wallWidth, float wallHeight, float thickness, const char* spriteFile);
    ~Wall();
    void Draw();
    bool CheckCollision(float ballX, float ballY, float& newX, float& newY, float& velX, float& velY);
};

#endif