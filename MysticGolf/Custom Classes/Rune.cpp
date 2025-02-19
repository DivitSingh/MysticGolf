#include "stdafx.h"
#include "Custom Classes/Rune.h"
#include <math.h>

Rune::Rune(float startX, float startY) {
    sprite = App::CreateSprite("./Resources/rune.png", 1, 1);
    glowSprite = App::CreateSprite("./Resources/runeGlow.png", 1, 1);
    x = startX;
    y = startY;
    radius = 32.0f;
    accelerationFactor = 1.5f;
    glowTimer = 0.0f;

    sprite->SetPosition(x, y);
    glowSprite->SetPosition(x, y);
    sprite->SetScale(1.0f);
    glowSprite->SetScale(1.0f);
}

Rune::~Rune() {
    delete sprite;
}

void Rune::Draw() {
    if (glowTimer > 0) {
        glowSprite->Draw();
    }
    else {
        sprite->Draw();
    }
}

void Rune::Update(float deltaTime) {
    if (glowTimer > 0) {
        glowTimer -= deltaTime;
    }
}

bool Rune::CheckCollision(float ballX, float ballY, float& velX, float& velY) {
    float dx = x - ballX;
    float dy = y - ballY;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance < radius) {
        // Apply acceleration but limit maximum velocity
        float newVelX = velX * accelerationFactor;
        float newVelY = velY * accelerationFactor;

        // Set new velocities with safety checks
        velX = newVelX;
        velY = newVelY;

        glowTimer = GLOW_DURATION;
        return true;
    }
    return false;
}
