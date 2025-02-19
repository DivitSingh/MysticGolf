#include "stdafx.h"
#include "Wall.h"

Wall::Wall(float startX, float startY, float wallWidth, float wallHeight, float thickness, const char* spriteFile) {
    x = startX;
    y = startY;
    width = wallWidth;
    height = wallHeight;

    // For obstacle walls (single cell)
    if (width == height && width <= 64.0f) {
        CSimpleSprite* sprite = App::CreateSprite(spriteFile, 1, 1);
        sprite->SetScale(thickness / sprite->GetHeight());
        sprite->SetPosition(x + width / 2, y + height / 2);
        sprites.push_back(sprite);
    }
    // For border walls (multiple sprites)
    else {
        // Calculate exact number of sprites needed
        int numSpritesNeeded;
        if (width > height) {
            numSpritesNeeded = ceil(width / thickness);
        }
        else {
            numSpritesNeeded = ceil(height / thickness);
        }

        for (int i = 0; i < numSpritesNeeded; i++) {
            CSimpleSprite* sprite = App::CreateSprite(spriteFile, 1, 1);
            sprite->SetScale(thickness / sprite->GetHeight());

            if (width > height) {
                // Horizontal wall - align exactly
                float xPos = x + (i * thickness) + thickness / 2;
                if (xPos + thickness / 2 > x + width) continue; // Skip if would exceed wall width
                sprite->SetPosition(xPos, y + thickness / 2);
            }
            else {
                // Vertical wall - align exactly
                float yPos = y + (i * thickness) + thickness / 2;
                if (yPos + thickness / 2 > y + height) continue; // Skip if would exceed wall height
                sprite->SetPosition(x + thickness / 2, yPos);
            }

            sprites.push_back(sprite);
        }
    }
}

Wall::~Wall() {
    for (auto sprite : sprites) {
        delete sprite;
    }
    sprites.clear();
}

void Wall::Draw() {
    for (auto sprite : sprites) {
        sprite->Draw();
    }
}

bool Wall::CheckCollision(float ballX, float ballY, float& newX, float& newY, float& velX, float& velY) {
    float ballRadius = 16.0f;  // Match ball's collision radius

    if (ballX + ballRadius >= x && ballX - ballRadius <= x + width &&
        ballY + ballRadius >= y && ballY - ballRadius <= y + height) {
        App::PlaySound("./Resources/wallHit.wav");
        float bounceCoef = -0.8f;

        if (ballX < x || ballX > x + width) {
            velX *= bounceCoef;
            newX = (ballX < x) ? x - ballRadius : x + width + ballRadius;
        }

        if (ballY < y || ballY > y + height) {
            velY *= bounceCoef;
            newY = (ballY < y) ? y - ballRadius : y + height + ballRadius;
        }

        return true;
    }
    return false;
}