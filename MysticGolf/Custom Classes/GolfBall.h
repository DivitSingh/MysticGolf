#ifndef GOLFBALL_H
#define GOLFBALL_H

#include "app/app.h"

class GolfBall {
private:
    CSimpleSprite* sprite;
    CSimpleSprite* immuneSprite;
    float x, y;
    float velocity_x, velocity_y;
    float drag;
    bool isMoving;
    bool isImmune;
    float animTimer;
    const float FRAME_DURATION = 0.1f;  // For faster/slower animation

public:
    GolfBall(float startX, float startY);
    ~GolfBall();
    void Update(float deltaTime);
    void Draw();
    void Hit(float power, float angle);
    void GetPosition(float& outX, float& outY) const;
    void SetPosition(float newX, float newY);
    void GetVelocity(float& outVelX, float& outVelY) const;
    void SetVelocity(float newVelX, float newVelY);
    void SetImmune(bool immune);
    bool IsStationary() const;
    void Reset();
};

#endif