#include "stdafx.h"
#include "GolfBall.h"
#include <math.h>

GolfBall::GolfBall(float startX, float startY) {
    sprite = App::CreateSprite("./Resources/ballSheet.png", 4, 1);
    immuneSprite = App::CreateSprite("./Resources/ballImmuneSheet.png", 4, 1);
    sprite->SetScale(1.0f);
    immuneSprite->SetScale(1.0f);

    sprite->SetPosition(startX, startY);
    immuneSprite->SetPosition(startX, startY);

    // Set up animations for both sprites
    sprite->CreateAnimation(0, 0.1f, { 0, 1, 2, 3 });
    immuneSprite->CreateAnimation(0, 0.1f, { 0, 1, 2, 3 });
    sprite->SetAnimation(0);
    immuneSprite->SetAnimation(0);

    isImmune = false;
    velocity_x = 0.0f;
    velocity_y = 0.0f;
    drag = 0.98f;
    isMoving = false;
}

GolfBall::~GolfBall() {
    delete sprite;
    delete immuneSprite;
}

void GolfBall::Update(float deltaTime) {
    float x, y;
    sprite->GetPosition(x, y);
    x += velocity_x;
    y += velocity_y;

    // Boundary collision with bounce
    if (x < 0) {
        x = 0;
        velocity_x = -velocity_x * 0.8f;  // Bounce with energy loss
    }
    if (x > APP_VIRTUAL_WIDTH) {
        x = APP_VIRTUAL_WIDTH;
        velocity_x = -velocity_x * 0.8f;
    }
    if (y < 0) {
        y = 0;
        velocity_y = -velocity_y * 0.8f;
    }
    if (y > APP_VIRTUAL_HEIGHT) {
        y = APP_VIRTUAL_HEIGHT;
        velocity_y = -velocity_y * 0.8f;
    }

    sprite->SetPosition(x, y);
    immuneSprite->SetPosition(x, y);

    // Apply drag
    velocity_x *= drag;
    velocity_y *= drag;

    float speed = sqrtf(velocity_x * velocity_x + velocity_y * velocity_y);
    isMoving = speed > 0.01f;

    // Update animation
    sprite->Update(deltaTime);
    immuneSprite->Update(deltaTime);
}

void GolfBall::Draw() {
    if (isImmune) {
        immuneSprite->Draw();
    }
    else {
        sprite->Draw();
    }
}

void GolfBall::Hit(float power, float angle) {
    float angleRad = angle * PI / 180.0f;
    velocity_x = power * cosf(angleRad);
    velocity_y = power * sinf(angleRad);
    isMoving = true;
}

void GolfBall::GetPosition(float& outX, float& outY) const {
    sprite->GetPosition(outX, outY);
}

void GolfBall::SetPosition(float newX, float newY) {
    sprite->SetPosition(newX, newY);
    immuneSprite->SetPosition(newX, newY);
}

void GolfBall::SetImmune(bool immune) {
    isImmune = immune;
}

bool GolfBall::IsStationary() const {
    return !isMoving;
}

void GolfBall::GetVelocity(float& outVelX, float& outVelY) const {
    outVelX = velocity_x;
    outVelY = velocity_y;
}

void GolfBall::SetVelocity(float newVelX, float newVelY) {
    velocity_x = newVelX;
    velocity_y = newVelY;
}

void GolfBall::Reset() {
    sprite->SetPosition(100, APP_VIRTUAL_HEIGHT / 2);  // Starting position
    velocity_x = 0.0f;
    velocity_y = 0.0f;
    isMoving = false;
}