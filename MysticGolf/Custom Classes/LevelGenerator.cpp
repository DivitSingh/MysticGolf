#include "stdafx.h"
#include "Custom Classes/LevelGenerator.h"
#include <random>
#include <queue>
#include <ctime>

LevelGenerator::LevelGenerator(int w, int h) : width(w), height(h) {
    // Seed random number generator with current time
    srand(static_cast<unsigned int>(time(nullptr)));

    // Add bounds checking
    if (w <= 0 || h <= 0) {
        width = 16;  // Default fallback values
        height = 12;
    }

    // Initialize grid with explicit size
    grid.resize(height);
    for (auto& row : grid) {
        row.resize(width);
    }

    // Initialize cells
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x].type = CellType::EMPTY;
            grid[y][x].worldX = x * CELL_SIZE;
            grid[y][x].worldY = y * CELL_SIZE;
            grid[y][x].isPath = false;
        }
    }
}

bool LevelGenerator::IsValidCell(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool LevelGenerator::IsSafeDistance(int x, int y, const GridCell& startCell) {
    float dx = x * CELL_SIZE - startCell.worldX;
    float dy = y * CELL_SIZE - startCell.worldY;
    float distance = sqrtf(dx * dx + dy * dy);
    return distance > 128.0f;  // Two cell widths for safety
}

void LevelGenerator::GenerateLevel() {
    PlaceStartAndHole();
    GenerateMainPath();
    PlaceMechanics();
}

void LevelGenerator::PlaceStartAndHole() {
    // Define the four corners (accounting for wall thickness)
    const std::pair<int, int> corners[4] = {
        {1, 1},                    // Bottom-left
        {1, height - 2},             // Top-left
        {width - 2, 1},              // Bottom-right
        {width - 2, height - 2}        // Top-right
    };

    // Randomly select a corner for start
    int startCorner = rand() % 4;
    int x = corners[startCorner].first;
    int y = corners[startCorner].second;

    // Place start point
    grid[y][x].type = CellType::START;

    // Place hole in opposite corner
    int holeCorner = (startCorner + 2) % 4;
    x = corners[holeCorner].first;
    y = corners[holeCorner].second;
    grid[y][x].type = CellType::HOLE;
}

void LevelGenerator::GenerateMainPath() {
    int startX = 0, startY = 0, endX = 0, endY = 0;

    // Find start and end positions
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x].type == CellType::START) {
                startX = x;
                startY = y;
            }
            if (grid[y][x].type == CellType::HOLE) {
                endX = x;
                endY = y;
            }
        }
    }

    int currentX = startX;
    int currentY = startY;

    while (currentX != endX || currentY != endY) {
        if (IsValidCell(currentX, currentY)) {
            grid[currentY][currentX].isPath = true;

            // Calculate next position
            int nextX = currentX + (currentX < endX ? 1 : (currentX > endX ? -1 : 0));
            int nextY = currentY + (currentY < endY ? 1 : (currentY > endY ? -1 : 0));

            if (IsValidCell(nextX, nextY)) {
                currentX = nextX;
                currentY = nextY;
            }
        }
    }
}

void LevelGenerator::PlaceMechanics() {
    int towerCount = 0;
    int wallCount = 0;
    int runeCount = 0;
    int whirlpoolCount = 0;
    int powerupCount = 0;

    const int MIN_TOWERS = 2;
    const int MAX_WALLS = 5;
    const int MAX_TOWERS = 4;
    const int MAX_RUNES = 5;
    const int MAX_WHIRLPOOLS = 3;
    const int MAX_POWERUPS = 2;

    // Get start cell and valid spots
    GridCell startCell;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x].type == CellType::START) {
                startCell = grid[y][x];
                break;
            }
        }
    }

    // Create distribution zones
    std::vector<std::pair<int, int>> validSpots;
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (grid[y][x].type == CellType::EMPTY &&
                IsSafeDistance(x, y, startCell)) {
                validSpots.push_back({ x, y });
            }
        }
    }
    std::random_shuffle(validSpots.begin(), validSpots.end());

    // First pass: Guarantee minimum tower count
    for (const auto& spot : validSpots) {
        if (towerCount >= MIN_TOWERS) break;

        int x = spot.first;
        int y = spot.second;
        grid[y][x].type = CellType::TOWER;
        towerCount++;
    }

    // Second pass: Place remaining elements
    for (const auto& spot : validSpots) {
        int x = spot.first;
        int y = spot.second;

        if (grid[y][x].type != CellType::EMPTY) continue;

        int roll = rand() % 100;
        if (roll < 20 && towerCount < MAX_TOWERS) {
            grid[y][x].type = CellType::TOWER;
            towerCount++;
        }
        if (roll < 40 && wallCount < MAX_WALLS) {
            grid[y][x].type = CellType::WALL;
            wallCount++;
        }
        else if (roll < 60 && runeCount < MAX_RUNES) {
            grid[y][x].type = CellType::RUNE;
            runeCount++;
        }
        else if (roll < 80 && whirlpoolCount < MAX_WHIRLPOOLS) {
            grid[y][x].type = CellType::WHIRLPOOL;
            whirlpoolCount++;
        }
        else if (roll < 100 && powerupCount < MAX_POWERUPS) {
            grid[y][x].type = CellType::POWERUP;
            powerupCount++;
        }
    }
}

const std::vector<std::vector<GridCell>>& LevelGenerator::GetGrid() const {
    return grid;
}