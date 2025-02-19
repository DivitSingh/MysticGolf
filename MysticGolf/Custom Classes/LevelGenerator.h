#ifndef LEVEL_GENERATOR_H
#define LEVEL_GENERATOR_H

#include <vector>

enum class CellType {
    EMPTY,
    WALL,
    START,
    HOLE,
    RUNE,
    WHIRLPOOL,
    POWERUP,
    TOWER
};

struct GridCell {
    CellType type;
    float worldX;
    float worldY;
    bool isPath;
};

class LevelGenerator {
private:
    std::vector<std::vector<GridCell>> grid;
    int width;
    int height;
    const float CELL_SIZE = 64.0f;

public:
    LevelGenerator(int w, int h);
    bool IsValidCell(int x, int y) const;
    bool IsSafeDistance(int x, int y, const GridCell& startCell);
    void GenerateLevel();
    void PlaceStartAndHole();
    void GenerateMainPath();
    void PlaceMechanics();
    const std::vector<std::vector<GridCell>>& GetGrid() const;
    static float GetCellSize() { return 64.0f; }
};

#endif