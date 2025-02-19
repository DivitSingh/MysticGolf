#ifndef AIMARROW_H
#define AIMARROW_H

#include "app/app.h"

class AimArrow {
private:
    CSimpleSprite* sprite;
    float baseScale;

public:
    AimArrow();
    ~AimArrow();
    void Draw(float x, float y, float angle, float power);
    void SetScale(float scale);
};

#endif