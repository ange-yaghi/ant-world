#include "../include/pathfinder.h"

#include "../include/game_object.h"

#include <algorithm>
#include <queue>

aw::Pathfinder::Pathfinder() {
    m_gridWidth = 0;
    m_gridUnit = 0;
    m_origin = ysMath::Constants::Zero;
}

aw::Pathfinder::~Pathfinder() {
    /* void */
}

void aw::Pathfinder::initialize(float gridUnit, int gridWidth) {
    m_gridUnit = gridUnit;
    m_gridWidth = gridWidth;

    m_grid = std::vector<std::vector<bool>>(m_gridWidth, std::vector<bool>(m_gridWidth));
}

void aw::Pathfinder::translate(const ysVector &v, int &gridX, int &gridY, bool clampOutput) const {
    ysVector d = ysMath::Sub(v, m_origin);
    
    float dx = (ysMath::GetX(d) / m_gridUnit) + 0.5f;
    float dy = (ysMath::GetY(d) / m_gridUnit) + 0.5f;

    gridX = (int)std::floor(dx) + m_gridWidth / 2;
    gridY = (int)std::floor(dy) + m_gridWidth / 2;

    if (clampOutput) {
        gridX = clamp(gridX, 0, m_gridWidth - 1);
        gridY = clamp(gridY, 0, m_gridWidth - 1);
    }
}

ysVector aw::Pathfinder::getLocation(int gridX, int gridY) const {
    int offsetX = gridX - m_gridWidth / 2;
    int offsetY = gridY - m_gridWidth / 2;

    ysVector offset = ysMath::LoadVector(offsetX * m_gridUnit, offsetY * m_gridUnit, 0.0f, 1.0f);

    return ysMath::Add(offset, m_origin);
}

void aw::Pathfinder::pathfind(const ysVector &destination, std::vector<ysVector> &path) const {
    int N = m_gridWidth;
    int N_nodes = N * N;
    std::vector<int> prev(N_nodes, -1);

    auto encode = [](int x, int y, int N) { return x + y * N; };
    auto decode = [](int d, int &x, int &y, int N) { x = (d % N); y = d / N; };

    int sx, sy, s;
    translate(destination, sx, sy);
    s = encode(sx, sy, N);

    int dx, dy, d;
    translate(m_origin, dx, dy);
    d = encode(dx, dy, N);

    std::queue<int> Q;
    Q.push(s);
    prev[s] = -2;

    while (!Q.empty()) {
        int u = Q.front(); Q.pop();
        if (u == d) break;

        int ux, uy;
        decode(u, ux, uy, N);

        // Dead end due to obstruction
        if (m_grid[uy][ux]) continue;

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue;
                if (i != 0 && j != 0) continue;

                int vx = ux + i;
                int vy = uy + j;

                if (vx >= 0 && vx < N && vy >= 0 && vy < N) {
                    int v = encode(vx, vy, N);
                    if (prev[v] == -1) {
                        prev[v] = u;
                        Q.push(v);
                    }
                }
            }
        }
    }

    if (prev[d] == -1) return;

    for (int i = d; i != -2; i = prev[i]) {
        int x, y;
        decode(i, x, y, N);

        path.push_back(getLocation(x, y));
    }
}

bool aw::Pathfinder::findObstacle(GameObject *obstacle) {
    bool found = false;
    for (GameObject *object: m_obstacles) {
        if (object == obstacle) {
            found = true;
            break;
        }
    }

    return found;
}

void aw::Pathfinder::addObstacle(GameObject *obstacle) {
    m_obstacles.push_back(obstacle);
}

void aw::Pathfinder::clearObstacles() {
    m_obstacles.clear();
}

void aw::Pathfinder::refreshGrid() {
    for (int i = 0; i < m_gridWidth; ++i) {
        for (int j = 0; j < m_gridWidth; ++j) {
            m_grid[j][i] = false;
        }
    }

    for (GameObject *object: m_obstacles) {
        int bounds = object->getPathfinderBoundCount();
        for (int i = 0; i < bounds; ++i) {
            int minX, minY;
            int maxX, maxY;

            ysVector minPoint = ysMath::Sub(
                object->getPathfinderBound(i).minPoint,
                ysMath::LoadVector(2.5f / 2, 2.5f / 2));
            ysVector maxPoint = ysMath::Add(
                object->getPathfinderBound(i).maxPoint,
                ysMath::LoadVector(2.5f / 2, 2.5f / 2));

            translate(minPoint, minX, minY, false);
            translate(maxPoint, maxX, maxY, false);

            if (minX >= m_gridWidth) continue;
            if (maxX < 0) continue;
            if (minY >= m_gridWidth) continue;
            if (maxY < 0) continue;

            minX = clamp(minX, 0, m_gridWidth - 1);
            maxX = clamp(maxX, 0, m_gridWidth - 1);
            minY = clamp(minY, 0, m_gridWidth - 1);
            maxY = clamp(maxY, 0, m_gridWidth - 1);

            for (int x = minX; x <= maxX; ++x) {
                for (int y = minY; y <= maxY; ++y) {
                    m_grid[y][x] = true;
                }
            }
        }
    }
}

int aw::clamp(int s, int l, int r) {
    if (s >= r) return r;
    else if (s <= l) return l;
    else return s;
}
