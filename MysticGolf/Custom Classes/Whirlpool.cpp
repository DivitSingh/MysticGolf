#include "stdafx.h"
#include "Whirlpool.h"
#include <math.h>

Whirlpool::Whirlpool(float startX, float startY) {
    pullSprite = App::CreateSprite("./Resources/whirlpoolSheet.png", 5, 8);
    pushSprite = App::CreateSprite("./Resources/whirlpoolReverseSheet.png", 5, 8);

    x = startX;
    y = startY;
    radius = 64.0f;
    strength = 1.5f;
    cycleTimer = 0.0f;
    isPushing = false;

    pullSprite->SetPosition(x, y);
    pushSprite->SetPosition(x, y);
    pullSprite->SetScale(1.0f);
    pushSprite->SetScale(1.0f);

    pullSprite->CreateAnimation(0, 0.1f, { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
                                        21,22,23,24,25,26,27,28,29,30,31,32,33,34,35 });
    pushSprite->CreateAnimation(0, 0.1f, { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
                                        21,22,23,24,25,26,27,28,29,30,31,32,33,34,35 });
    pullSprite->SetAnimation(0);
    pushSprite->SetAnimation(0);
}

Whirlpool::~Whirlpool() {
    delete pullSprite;
    delete pushSprite;
}

void Whirlpool::Update(float deltaTime) {
    pullSprite->Update(deltaTime);
    pushSprite->Update(deltaTime);

    float deltaSeconds = deltaTime / 1000.0f;  // Convert ms to seconds
    cycleTimer += deltaSeconds;
    if (cycleTimer >= CYCLE_DURATION) {
        cycleTimer = 0.0f;
        isPushing = !isPushing;
    }
}

bool Whirlpool::CheckEffect(float ballX, float ballY, float& velX, float& velY) {
    float dx = x - ballX;
    float dy = y - ballY;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance < radius) {
        float dirX = dx / distance;
        float dirY = dy / distance;

        float force = strength * (isPushing ? -1.5f : 1.0f);

        velX += dirX * force;
        velY += dirY * force;
        return true;
    }
    return false;
}

void Whirlpool::Draw() {
    if (isPushing) {
        pushSprite->Draw();
    }
    else {
        pullSprite->Draw();
    }
}