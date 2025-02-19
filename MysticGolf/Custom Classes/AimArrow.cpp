#include "stdafx.h"
#include "AimArrow.h"

AimArrow::AimArrow() {
    sprite = App::CreateSprite("./Resources/arrow.png", 1, 1);
    baseScale = 1.0f;
    sprite->SetScale(baseScale);
}

AimArrow::~AimArrow() {
    delete sprite;
}

void AimArrow::Draw(float x, float y, float angle, float power) {
    float offset = 32.0f;  // Adjust this value based on sprites
    float angleRad = angle * PI / 180.0f;
    float offsetX = x + offset * cosf(angleRad);
    float offsetY = y + offset * sinf(angleRad);

    sprite->SetPosition(offsetX, offsetY);
    sprite->SetAngle(angle * PI / 180.0f);
    sprite->SetScale(baseScale * (power / 10.0f));  // Scale with power
    sprite->Draw();
}

void AimArrow::SetScale(float scale) {
    baseScale = scale;
}