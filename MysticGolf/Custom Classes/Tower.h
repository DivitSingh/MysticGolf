#ifndef TOWER_H
#define TOWER_H

#include "App/app.h"

enum TowerAnimations {
    ANIM_DESTRUCTION = 0
};

class Tower {
private:
    CSimpleSprite* towerSprite;
    CSimpleSprite* destroyedSprite;
    CSimpleSprite* destructionAnim;
    float x, y;
    bool isDestroyed;
    bool isAnimating;
    float animTimer;
    static constexpr float ANIM_DURATION = 1.0f;
    static constexpr float COLLISION_RADIUS = 32.0f;

public:
    Tower(float startX, float startY);
    ~Tower();
    void Update(float deltaTime);
    void Draw();
    bool CheckCollision(float ballX, float ballY);
    bool IsDestroyed() const { return isDestroyed; }
};

#endif