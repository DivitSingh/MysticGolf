#ifndef HOLE_H
#define HOLE_H

#include "app/app.h"

class Hole {
private:
    CSimpleSprite* sprite;
    float x, y;
    float radius;
    bool isVisible;

public:
    Hole(float startX, float startY);
    ~Hole();
    void Draw();
    bool CheckBallInHole(float ballX, float ballY);
    bool IsVisible() { return isVisible; }
    void SetVisible(bool visible) { isVisible = visible; }
};

#endif