#ifndef POWERUP_MANAGER_H
#define POWERUP_MANAGER_H

#include "app/app.h"
#include <vector>

class PowerupItem {
private:
    CSimpleSprite* sprite;
    float x, y;
    bool isCollected;

public:
    PowerupItem(float startX, float startY);
    ~PowerupItem();
    void Draw();
    bool CheckCollision(float ballX, float ballY);
    bool IsCollected() const { return isCollected; }
};

class PowerupManager {
private:
    std::vector<PowerupItem*> powerups;
    int collectedCount;
    float immunityTimer;
    bool isImmunityActive;
    const float IMMUNITY_DURATION = 10.0f;  // in seconds

public:
    PowerupManager();
    ~PowerupManager();
    void AddPowerup(float x, float y);
    void Update(float deltaTime);
    void Draw();
    bool CheckCollisions(float ballX, float ballY);
    void ActivateImmunity();
    bool IsImmune() const { return isImmunityActive; }
};

#endif