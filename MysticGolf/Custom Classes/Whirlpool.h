#ifndef WHIRLPOOL_H
#define WHIRLPOOL_H

#include "app/app.h"

class Whirlpool {
private:
    CSimpleSprite* pullSprite;
    CSimpleSprite* pushSprite;
    float x, y;
    float radius;
    float strength;
    float cycleTimer;
    bool isPushing;
    const float CYCLE_DURATION = 2.5f;

public:
    Whirlpool(float startX, float startY);
    ~Whirlpool();
    void Update(float deltaTime);
    void Draw();
    bool CheckEffect(float ballX, float ballY, float& velX, float& velY);
};

#endif