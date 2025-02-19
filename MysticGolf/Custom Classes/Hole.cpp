#include "stdafx.h"
#include "Hole.h"
#include <math.h>

Hole::Hole(float startX, float startY) {
    sprite = App::CreateSprite("./Resources/hole.png", 1, 1);
    sprite->SetScale(1.0f);
    x = startX;
    y = startY;
    sprite->SetPosition(x, y);
    radius = 32.0f;  // Adjust based on sprite size
    isVisible = false;
}

Hole::~Hole() {
    delete sprite;
}

void Hole::Draw() {
    if (isVisible) {
        sprite->Draw();
    }
}

bool Hole::CheckBallInHole(float ballX, float ballY) {
    if (!isVisible) return false;
    float dx = x - ballX;
    float dy = y - ballY;
    float distance = sqrtf(dx * dx + dy * dy);
    return distance < radius;
}