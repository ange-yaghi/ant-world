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
                newFloor->RigidBody.Transform.SetPosition(
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
                    newWall->RigidBody.Transform.SetPosition(
                        ysMath::LoadVector((i - center_x) * param.GridSize, (j - center_y) * param.GridSize, 0.0f, 0.0f));
                }
            }
        }
    }
}

void aw::InteriorGenerator::generateRectangle(Realm *realm, float width, float height) {
    ysVector center = ysMath::Constants::Zero;
    ysVector lw = ysMath::Add(center, ysMath::LoadVector(-width / 2.0f - 0.5f));
    ysVector rw = ysMath::Add(center, ysMath::LoadVector(width / 2.0f + 0.5f));
    ysVector tw = ysMath::Add(center, ysMath::LoadVector(0.0f, height / 2.0f + 0.5f));
    ysVector bw = ysMath::Add(center, ysMath::LoadVector(0.0f, -height / 2.0f - 0.5f));

    generateWall(realm, lw, 1.0f, height, 10.0f, true);
    generateWall(realm, rw, 1.0f, height, 10.0f, true);
    generateWall(realm, tw, 1.0f, width, 10.0f, false);
    generateWall(realm, bw, 1.0f, width, 10.0f, false);

    Flooring *newFloor = realm->spawn<Flooring>();
    newFloor->setDimensions(width, height);
    newFloor->RigidBody.Transform.SetPosition(center);
}

void aw::InteriorGenerator::generateWall(
    Realm *realm, const ysVector &center, float thickness, float length, float unitSize, bool vertical) 
{
    ysVector delta = vertical
        ? ysMath::LoadVector(0.0f, 1.0f, 0.0f)
        : ysMath::LoadVector(1.0f, 0.0f, 0.0f);

    ysVector start = vertical
        ? ysMath::LoadVector(0.0f, -length / 2.0f, 0.0f)
        : ysMath::LoadVector(-length / 2.0f, 0.0f, 0.0f);

    start = ysMath::Add(center, start);

    float currentLength = 0.0f;
    int unitCount = 0;
    while (currentLength < length) {
        Wall *newWall = realm->spawn<Wall>();
        float thisLength = min(unitSize, length - currentLength);
        if (vertical) {
            newWall->setDimensions(thickness, thisLength);
        }
        else {
            newWall->setDimensions(thisLength, thickness);
        }

        currentLength += unitSize;

        ysVector basePosition = ysMath::Add(start, ysMath::Mul(delta, ysMath::LoadScalar(unitCount * unitSize)));
        ysVector offset = ysMath::Mul(ysMath::LoadScalar(thisLength / 2.0f), delta);

        newWall->RigidBody.Transform.SetPosition(
            ysMath::Add(basePosition, offset));

        ++unitCount;
    }
}
