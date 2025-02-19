#include "stdafx.h"
#include "Custom Classes/Tower.h"
#include <math.h>

Tower::Tower(float startX, float startY) : x(startX), y(startY) {
    // Tower sprite is 32x64
    towerSprite = App::CreateSprite("./Resources/tower.png", 1, 1);
    destroyedSprite = App::CreateSprite("./Resources/towerDestroyed.png", 1, 1);

    // Explosion animation is 128x128 with 4x4 frames of 32x32 each
    destructionAnim = App::CreateSprite("./Resources/explosionSheet.png", 4, 4);

    towerSprite->SetPosition(x, y);
    destroyedSprite->SetPosition(x, y);
    destructionAnim->SetPosition(x, y);

    // Set scale for tall tower sprites (32x64)
    towerSprite->SetScale(1.0f);
    destroyedSprite->SetScale(1.0f);

    // Animation setup matching Whirlpool pattern
    destructionAnim->CreateAnimation(ANIM_DESTRUCTION, 1.0f / 12.0f, { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 });
    destructionAnim->SetAnimation(ANIM_DESTRUCTION);

    isDestroyed = false;
    isAnimating = false;
    animTimer = 0.0f;
}

Tower::~Tower() {
    delete towerSprite;
    delete destroyedSprite;
    delete destructionAnim;
}

void Tower::Update(float deltaTime) {
    if (isAnimating) {
        destructionAnim->Update(deltaTime);
        animTimer += deltaTime / 1000.0f;

        if (animTimer >= ANIM_DURATION) {
            isAnimating = false;
            isDestroyed = true;
        }
    }
}

void Tower::Draw() {
    if (isAnimating) {
        destructionAnim->Draw();
    }
    else if (isDestroyed) {
        destroyedSprite->Draw();
    }
    else {
        towerSprite->Draw();
    }
}

bool Tower::CheckCollision(float ballX, float ballY) {
    if (isDestroyed || isAnimating) return false;

    float dx = x - ballX;
    float dy = y - ballY;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance < COLLISION_RADIUS) {
        App::PlaySound("./Resources/towerDestroy.wav");
        isAnimating = true;
        animTimer = 0.0f;
        return true;
    }
    return false;
}
