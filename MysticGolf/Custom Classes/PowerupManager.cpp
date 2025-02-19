#include "stdafx.h"
#include "Custom Classes/PowerupManager.h"
#include <math.h>

PowerupItem::PowerupItem(float startX, float startY) {
    sprite = App::CreateSprite("./Resources/powerup.png", 1, 1);
    x = startX + 16.0f;
    y = startY + 16.0f;
    isCollected = false;
    sprite->SetPosition(x, y);
    sprite->SetScale(1.0f);
}

PowerupItem::~PowerupItem() {
    delete sprite;
}

void PowerupItem::Draw() {
    if (!isCollected) {
        sprite->Draw();
    }
}

bool PowerupItem::CheckCollision(float ballX, float ballY) {
    if (isCollected) return false;

    float dx = x - ballX;
    float dy = y - ballY;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance < 32.0f) {
        App::PlaySound("./Resources/collect.wav");
        isCollected = true;
        return true;
    }
    return false;
}

PowerupManager::PowerupManager() {
    collectedCount = 0;
    immunityTimer = 0.0f;
    isImmunityActive = false;
}

PowerupManager::~PowerupManager() {
    for (auto powerup : powerups) {
        delete powerup;
    }
    powerups.clear();
}

void PowerupManager::AddPowerup(float x, float y) {
    powerups.push_back(new PowerupItem(x, y));
}

void PowerupManager::Update(float deltaTime) {
    float deltaSeconds = deltaTime / 1000.0f;

    if (isImmunityActive) {
        immunityTimer -= deltaSeconds;
        if (immunityTimer <= 0) {
            isImmunityActive = false;
        }
    }

    // Remove collected powerups
    auto it = powerups.begin();
    while (it != powerups.end()) {
        if ((*it)->IsCollected()) {
            delete* it;
            it = powerups.erase(it);
        }
        else {
            ++it;
        }
    }
}

void PowerupManager::Draw() {
    // Draw powerups
    for (auto powerup : powerups) {
        powerup->Draw();
    }

    // Draw UI showing collected powerups and immunity status
    char powerupText[32];
    sprintf_s(powerupText, "Powerups: %d", collectedCount);
    App::Print(0, APP_VIRTUAL_HEIGHT - 35, powerupText, 1, 1, 1, GLUT_BITMAP_9_BY_15);

    if (isImmunityActive) {
        char immunityText[32];
        sprintf_s(immunityText, "IMMUNE: %.1f", immunityTimer);
        App::Print(0, APP_VIRTUAL_HEIGHT - 55, immunityText, 1, 0, 0, GLUT_BITMAP_9_BY_15);
    }
}

bool PowerupManager::CheckCollisions(float ballX, float ballY) {
    for (auto powerup : powerups) {
        if (powerup->CheckCollision(ballX, ballY)) {
            collectedCount++;
            return true;
        }
    }
    return false;
}

void PowerupManager::ActivateImmunity() {
    if (collectedCount > 0 && !isImmunityActive) {
        App::PlaySound("./Resources/activate.wav");
        collectedCount--;
        isImmunityActive = true;
        immunityTimer = IMMUNITY_DURATION;
    }
}