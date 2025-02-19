#include "stdafx.h"
#include "app/app.h"
#include <algorithm>
#include "Custom Classes/GolfBall.h"
#include "Custom Classes/AimArrow.h"
#include "Custom Classes/Hole.h"
#include "Custom Classes/Wall.h"
#include "Custom Classes/Tower.h"
#include "Custom Classes/Rune.h"
#include "Custom Classes/Whirlpool.h"
#include "Custom Classes/PowerupManager.h"
#include "Custom Classes/LevelGenerator.h"

enum GameState {
    INTRO,
    PLAYING
};

GameState currentState = INTRO;

GolfBall* ball;
AimArrow* aimArrow;
Hole* hole;
CSimpleSprite* backgroundTile;

std::vector<CSimpleSprite*> backgroundTiles;
std::vector<Wall*> borderWalls;
std::vector<Wall*> obstacleWalls;
std::vector<Tower*> towers;
std::vector<Rune*> runes;
std::vector<Whirlpool*> whirlpools;

PowerupManager* powerupManager;
LevelGenerator* levelGenerator;

float aimAngle = 0.0f;
float power = 5.0f;
const float MIN_POWER = 1.0f;
const float MAX_POWER = 10.0f;
int shotCount = 0;

void InitializeStaticElements() {
    // Background tiles
    int tilesX = APP_VIRTUAL_WIDTH / 64;
    int tilesY = APP_VIRTUAL_HEIGHT / 64;
    for (int y = 0; y < tilesY; y++) {
        for (int x = 0; x < tilesX; x++) {
            CSimpleSprite* tile = App::CreateSprite("./Resources/ground.png", 1, 1);
            tile->SetPosition(x * 64.0f + 32.0f, y * 64.0f + 32.0f);
            backgroundTiles.push_back(tile);
        }
    }

    // Core UI and permanent elements
    aimArrow = new AimArrow();

    // Walls
    int wallThickness = 32;
    borderWalls.push_back(new Wall(0, 0, APP_VIRTUAL_WIDTH, wallThickness, wallThickness, "./Resources/wall.png"));
    borderWalls.push_back(new Wall(0, APP_VIRTUAL_HEIGHT - wallThickness, APP_VIRTUAL_WIDTH, wallThickness, wallThickness, "./Resources/wall.png"));
    borderWalls.push_back(new Wall(0, 0, wallThickness, APP_VIRTUAL_HEIGHT, wallThickness, "./Resources/wall.png"));
    borderWalls.push_back(new Wall(APP_VIRTUAL_WIDTH - wallThickness, 0, wallThickness, APP_VIRTUAL_HEIGHT, wallThickness, "./Resources/wall.png"));
}

void InitializeDynamicElements() {
    levelGenerator = new LevelGenerator(16, 12);
    levelGenerator->GenerateLevel();
    const auto& grid = levelGenerator->GetGrid();
    powerupManager = new PowerupManager();

    // Spawn objects based on grid assignments in LevelGenerator
    if (!grid.empty() && !grid[0].empty()) {
        for (int y = 0; y < grid.size(); y++) {
            for (int x = 0; x < grid[0].size(); x++) {
                const GridCell& cell = grid[y][x];
                switch (cell.type) {
                case CellType::START:
                    ball = new GolfBall(cell.worldX, cell.worldY);
                    break;
                case CellType::HOLE:
                    hole = new Hole(cell.worldX, cell.worldY);
                    break;
                case CellType::WALL:
                    obstacleWalls.push_back(new Wall(cell.worldX - LevelGenerator::GetCellSize() / 2, cell.worldY - LevelGenerator::GetCellSize() / 2,
                        LevelGenerator::GetCellSize(), LevelGenerator::GetCellSize(), LevelGenerator::GetCellSize(), "./Resources/crate.png"));
                    break;
                case CellType::TOWER:
                    towers.push_back(new Tower(cell.worldX, cell.worldY));
                    break;
                case CellType::RUNE:
                    runes.push_back(new Rune(cell.worldX, cell.worldY));
                    break;
                case CellType::WHIRLPOOL:
                    whirlpools.push_back(new Whirlpool(cell.worldX, cell.worldY));
                    break;
                case CellType::POWERUP:
                    powerupManager->AddPowerup(cell.worldX, cell.worldY);
                    break;
                }
            }
        }
    }
}

// Clears Static Elements that are not randomized
void CleanupStaticElements() {
    delete aimArrow;

    for (auto tile : backgroundTiles) {
        delete tile;
    }
    backgroundTiles.clear();

    for (Wall* wall : borderWalls) {
        delete wall;
    }
    borderWalls.clear();
}

// Clears Dynamic Elements that are randomized each round
void CleanupDynamicElements() {
    delete ball;
    delete hole;

    for (auto wall : obstacleWalls) {
        delete wall;
    }
    obstacleWalls.clear();
    for (auto tower : towers) {
        delete tower;
    }
    towers.clear();
    for (auto rune : runes) {
        delete rune;
    }
    runes.clear();
    for (auto whirlpool : whirlpools) {
        delete whirlpool;
    }
    whirlpools.clear();

    delete powerupManager;
    delete levelGenerator;
}

void Init() {
    InitializeStaticElements();
    InitializeDynamicElements();
}

void ProcessInput() {
    // Aim Controls (Keyboard)
    if (App::IsKeyPressed(VK_LEFT)) {
        aimAngle += 2.0f;
    }
    if (App::IsKeyPressed(VK_RIGHT)) {
        aimAngle -= 2.0f;
    }

    // Aim Controls with Left Stick
    float stickX = App::GetController().GetLeftThumbStickX();
    float stickY = App::GetController().GetLeftThumbStickY();
    if (abs(stickX) > 0.1f || abs(stickY) > 0.1f) {
        aimAngle = atan2f(stickY, stickX) * 180.0f / PI;
    }

    // Power Controls (Keyboard)
    if (App::IsKeyPressed(VK_UP)) {
        power = min(power + 0.1f, MAX_POWER);
    }
    if (App::IsKeyPressed(VK_DOWN)) {
        power = max(power - 0.1f, MIN_POWER);
    }

    // Power Control with Right Stick
    float powerStick = App::GetController().GetRightThumbStickY();
    if (abs(powerStick) > 0.1f) {
        power = max(MIN_POWER, min(MAX_POWER, power + powerStick * 0.2f));
    }

    if (App::IsKeyPressed(VK_SPACE) || App::GetController().CheckButton(XINPUT_GAMEPAD_A)) {
        ball->Hit(power, aimAngle);
        App::PlaySound("./Resources/ballHit.wav");
        aimAngle = 0.0f;
        power = 5.0f;
        shotCount++;
    }
}

// Updates all game elements
void UpdateGameElements(float deltaTime) {
    powerupManager->Update(deltaTime);

    for (auto whirlpool : whirlpools) {
        whirlpool->Update(deltaTime);
    }

    for (auto rune : runes) {
        rune->Update(deltaTime);
    }

    for (auto tower : towers) {
        tower->Update(deltaTime);
    }
}

void CheckCollisions() {
    float ballX, ballY, velX, velY;
    ball->GetPosition(ballX, ballY);
    ball->GetVelocity(velX, velY);

    // Powerup collisions
    powerupManager->CheckCollisions(ballX, ballY);

    // Whirlpool effects
    if (!powerupManager->IsImmune()) {
        for (auto whirlpool : whirlpools) {
            if (whirlpool->CheckEffect(ballX, ballY, velX, velY)) {
                ball->SetVelocity(velX, velY);
            }
        }
    }

    // Rune collisions
    for (auto rune : runes) {
        if (rune->CheckCollision(ballX, ballY, velX, velY)) {
            ball->SetVelocity(velX, velY);
        }
    }

    // Wall collisions
    for (Wall* wall : borderWalls) {
        float newX = ballX;
        float newY = ballY;
        if (wall->CheckCollision(ballX, ballY, newX, newY, velX, velY)) {
            ball->SetPosition(newX, newY);
            ball->SetVelocity(velX, velY);
        }
    }

    for (Wall* wall : obstacleWalls) {
        float newX = ballX;
        float newY = ballY;
        if (wall->CheckCollision(ballX, ballY, newX, newY, velX, velY)) {
            ball->SetPosition(newX, newY);
            ball->SetVelocity(velX, velY);
        }
    }

    // Tower collisions
    for (auto tower : towers) {
        tower->CheckCollision(ballX, ballY);
    }
}

void UpdateGameState() {
    // Update hole visibility based on towers left (no towers spawns the hole)
    bool allTowersDestroyed = std::all_of(towers.begin(), towers.end(),
        [](const Tower* t) { return t->IsDestroyed(); });
    if (allTowersDestroyed && !hole->IsVisible()) {
        hole->SetVisible(true);
        App::PlaySound("./Resources/holeAppear.wav");
    }
    hole->SetVisible(allTowersDestroyed);

    // Update ball immunity
    ball->SetImmune(powerupManager->IsImmune());

    // Check win condition
    float ballX, ballY;
    ball->GetPosition(ballX, ballY);
    if (hole->CheckBallInHole(ballX, ballY) && ball->IsStationary()) {
        App::PlaySound("./Resources/win.wav");
        CleanupDynamicElements();
        InitializeDynamicElements();
        aimAngle = 0.0f;
        power = 5.0f;
        shotCount = 0;
    }
}


void Update(float deltaTime) {
    if (currentState == INTRO) {
        if (App::GetController().CheckButton(XINPUT_GAMEPAD_X) || App::IsKeyPressed(VK_SHIFT)) {
            currentState = PLAYING;
        }
        return;
    }
    else
    {
        // Main Update Loop calling Helper Functions above
        if (ball->IsStationary()) {
            ProcessInput();
        }

        UpdateGameElements(deltaTime);
        CheckCollisions();
        UpdateGameState();
        ball->Update(deltaTime);

        // Powerup Activation Check
        if (App::GetController().CheckButton(XINPUT_GAMEPAD_X) || App::IsKeyPressed(VK_SHIFT)) {
            powerupManager->ActivateImmunity();
        }
    }
}

void Render()
{
    if (currentState == INTRO) {
        App::Print(0, APP_VIRTUAL_HEIGHT - 15, "MYSTIC GOLF", 0, 1, 1, GLUT_BITMAP_9_BY_15);
        App::Print(0, APP_VIRTUAL_HEIGHT - 45, "Arrow Keys or Left Stick to aim", 0, 1, 1, GLUT_BITMAP_8_BY_13);
        App::Print(0, APP_VIRTUAL_HEIGHT - 60, "Up/Down or Right Stick to adjust power", 0, 1, 1, GLUT_BITMAP_8_BY_13);
        App::Print(0, APP_VIRTUAL_HEIGHT - 75, "Space/A to shoot", 0, 1, 1, GLUT_BITMAP_8_BY_13);
        App::Print(0, APP_VIRTUAL_HEIGHT - 90, "Shift/X to activate powerup immunity", 0, 1, 1, GLUT_BITMAP_8_BY_13);
        App::Print(0, APP_VIRTUAL_HEIGHT - 120, "Steer the Divine Ball through the Ritual Grounds.", 0, 1, 1, GLUT_BITMAP_8_BY_13);
        App::Print(0, APP_VIRTUAL_HEIGHT - 135, "Destroy all Occultation Towers to reveal the Summoning Circle.", 0, 1, 1, GLUT_BITMAP_8_BY_13);
        App::Print(0, APP_VIRTUAL_HEIGHT - 150, "Guide the Divine Ball into the Summoning Circle to end the profane ritual.", 0, 1, 1, GLUT_BITMAP_8_BY_13);
        App::Print(0, APP_VIRTUAL_HEIGHT - 165, "Obstacles such as Whirlpools will suck or repel the Divine Ball based on their current color.", 0, 1, 1, GLUT_BITMAP_8_BY_13);
        App::Print(0, APP_VIRTUAL_HEIGHT - 180, "Runes speed up the Divine Ball when it comes into contact.", 0, 1, 1, GLUT_BITMAP_8_BY_13);
        App::Print(0, APP_VIRTUAL_HEIGHT - 195, "Holy Water can be collected and activated when needed to Empower the Divine Ball.", 0, 1, 1, GLUT_BITMAP_8_BY_13);
        App::Print(0, APP_VIRTUAL_HEIGHT - 210, "When Empowered, the Divine Ball turns Golden and is immune to Whirlpools.", 0, 1, 1, GLUT_BITMAP_8_BY_13);
        App::Print(0, APP_VIRTUAL_HEIGHT - 240, "Press Shift or X to start!", 0, 1, 1, GLUT_BITMAP_8_BY_13);
        return;
    }
    else
    {
        for (auto tile : backgroundTiles) {
            tile->Draw();
        }
        for (Wall* wall : borderWalls) {
            wall->Draw();
        }
        for (Wall* wall : obstacleWalls) {
            wall->Draw();
        }
        for (auto tower : towers) {
            tower->Draw();
        }
        for (auto rune : runes) {
            rune->Draw();
        }
        for (auto whirlpool : whirlpools) {
            whirlpool->Draw();
        }

        ball->Draw();
        hole->Draw();
        powerupManager->Draw();

        // Draw Aim Arrow only when Ball is stationary
        if (ball->IsStationary()) {
            float x, y;
            ball->GetPosition(x, y);
            aimArrow->Draw(x, y, aimAngle, power);
        }

        char shotText[32];
        sprintf_s(shotText, "Shots: %d", shotCount);
        App::Print(0, APP_VIRTUAL_HEIGHT - 15, shotText, 1, 1, 1, GLUT_BITMAP_9_BY_15);
    }
}

void Shutdown()
{
    CleanupStaticElements();
    CleanupDynamicElements();
}