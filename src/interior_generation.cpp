#include "../include/interior_generation.h"

#include "../include/wall.h"
#include "../include/realm.h"
#include "../include/flooring.h"

#include <algorithm>
#include <cmath>
#include <vector>
#include <random>

aw::InteriorGenerator::InteriorGenerator() {
    /* void */
}

aw::InteriorGenerator::~InteriorGenerator() {
    /* void */
}

void aw::InteriorGenerator::generateWalls(Realm *realm, const WallsParam &param) {
    int N = (int)std::ceil(std::sqrt((double)param.MaxCells)) * 2;

    std::vector<std::vector<bool>> grid(N, std::vector<bool>(N, false));
    int center_x = N / 2;
    int center_y = N / 2;

    std::vector<std::pair<int, int>> cells;
    cells.push_back({ center_x, center_y });
    grid[center_x][center_y] = true;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> cellSelector(0, param.MaxCells);
    std::uniform_int_distribution<int> directionSelector(0, param.MaxCells);
    std::uniform_int_distribution<int> earlyExitSelector(0, 9);

    while (cells.size() < param.MaxCells) {
        int cellCount = cells.size();
        int cell = cellSelector(generator);
        if (cell >= cellCount) continue;

        int cell_x = cells[cell].first;
        int cell_y = cells[cell].second;

        int dir = directionSelector(generator);
        if (dir == 0) {
            ++cell_x;
        }
        else if (dir == 1) {
            --cell_x;
        }
        else if (dir == 2) {
            ++cell_y;
        }
        else if (dir == 3) {
            --cell_y;
        }

        if (cell_x >= N - 1 || cell_x <= 0) continue;
        if (cell_y >= N - 1 || cell_y <= 0) continue;

        if (!grid[cell_y][cell_x]) {
            // 1/10 chance of early exit
            if (earlyExitSelector(generator) == 0) break;

            grid[cell_y][cell_x] = true;
            cells.push_back({ cell_x, cell_y });
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (grid[j][i]) {
                Flooring *newFloor = realm->spawn<Flooring>();
                newFloor->setDimensions(param.GridSize, param.GridSize);
                newFloor->RigidBody.SetPosition(
                    ysMath::LoadVector((i - center_x) * param.GridSize, (j - center_y) * param.GridSize, 0.0f, 0.0f));
            }
            else {
                bool border = false;
                if (j > 0 && grid[j - 1][i]) border = true;
                if (j < N - 1 && grid[j + 1][i]) border = true;
                if (i > 0 && grid[j][i - 1]) border = true;
                if (i < N - 1 && grid[j][i + 1]) border = true;

                if (border) {
                    Wall *newWall = realm->spawn<Wall>();
                    newWall->setDimensions(param.GridSize, param.GridSize);
                    newWall->RigidBody.SetPosition(
                        ysMath::LoadVector((i - center_x) * param.GridSize, (j - center_y) * param.GridSize, 0.0f, 0.0f));
                }
            }
        }
    }
}
