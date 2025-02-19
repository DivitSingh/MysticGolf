#ifndef RUNE_H
#define RUNE_H

#include "app/app.h"

class Rune {
private:
    CSimpleSprite* sprite;
    CSimpleSprite* glowSprite;
    float x, y;
    float radius;
    float accelerationFactor;
    float glowTimer;
    const float GLOW_DURATION = 1.0f;

public:
    Rune(float startX, float startY);
    ~Rune();
    void Draw();
    void Update(float deltaTime);
    bool CheckCollision(float ballX, float ballY, float& velX, float& velY);
};

#endif