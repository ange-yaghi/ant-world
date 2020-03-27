#include "../include/pathfinder.h"

#include "../include/game_object.h"

#include <algorithm>
#include <queue>

aw::Pathfinder::Pathfinder() {
    m_gridWidth = 0;
    m_gridUnit = 0;
    m_origin = ysMath::Constants::Zero;
    m_destination = ysMath::Constants::Zero;
    m_pathIndex = 0;
    m_evading = false;
    m_grid = nullptr;
}

aw::Pathfinder::~Pathfinder() {
    /* void */
}

void aw::Pathfinder::initialize(float gridUnit, int gridWidth) {
    m_gridUnit = gridUnit;
    m_gridWidth = gridWidth;

    m_grid = new bool *[m_gridWidth];
    for (int i = 0; i < m_gridWidth; ++i) {
        m_grid[i] = new bool[m_gridWidth];
    }
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

void aw::Pathfinder::clearPath() {
    m_pathIndex = 0;
    m_path.clear();
}

bool aw::Pathfinder::evade(std::vector<int> &path) const {
    int N = m_gridWidth;
    int N_nodes = N * N;
    std::vector<int> prev(N_nodes, -1);

    int sx, sy, s;
    translate(m_origin, sx, sy);
    s = encode(sx, sy);

    int d = -1;

    std::queue<int> Q;
    Q.push(s);
    prev[s] = -2;

    while (!Q.empty()) {
        int u = Q.front(); Q.pop();
        if (u == d) break;

        int ux, uy;
        decode(u, ux, uy);

        // Early Exit
        if (!m_grid[uy][ux]) {
            d = u;
            break;
        }

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue;
                if (i != 0 && j != 0) continue;

                int vx = ux + i;
                int vy = uy + j;

                if (vx >= 0 && vx < N && vy >= 0 && vy < N) {
                    int v = encode(vx, vy);
                    if (prev[v] == -1) {
                        prev[v] = u;
                        Q.push(v);
                    }
                }
            }
        }
    }

    if (d == -1) return false;

    for (int i = d; i != s; i = prev[i]) {
        path.push_back(i);
    }

    return true;
}

bool aw::Pathfinder::pathfind(const ysVector &destination, std::vector<int> &path) const {
    int N = m_gridWidth;
    int N_nodes = N * N;
    std::vector<int> prev(N_nodes, -1);

    int sx, sy, s;
    translate(m_origin, sx, sy);
    s = encode(sx, sy);

    int dx, dy, d;
    translate(m_destination, dx, dy);
    d = encode(dx, dy);

    std::queue<int> Q;
    Q.push(s);
    prev[s] = -2;

    int closestToDestination = -1;
    int smallestDistanceFromDestination = INT_MAX;

    while (!Q.empty()) {
        int u = Q.front(); Q.pop();
        if (u == d) break;

        int ux, uy;
        decode(u, ux, uy);

        // Dead end due to obstruction
        if (m_grid[uy][ux]) continue;

        int distance = std::abs(dx - ux) + std::abs(dy - uy);
        if (distance < smallestDistanceFromDestination) {
            smallestDistanceFromDestination = distance;
            closestToDestination = u;
        }

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue;
                if (i != 0 && j != 0) continue;

                int vx = ux + i;
                int vy = uy + j;

                if (vx >= 0 && vx < N && vy >= 0 && vy < N) {
                    int v = encode(vx, vy);
                    if (prev[v] == -1) {
                        prev[v] = u;
                        Q.push(v);
                    }
                }
            }
        }
    }

    if (prev[d] == -1 && closestToDestination == -1) return false;

    int newDestination = (prev[d] == -1)
        ? closestToDestination
        : d;

    for (int i = newDestination; i != -2; i = prev[i]) {
        path.push_back(i);
    }

    std::reverse(path.begin(), path.end());

    return true;
}

void aw::Pathfinder::removeObstacle(int index) {
    m_obstacles[index]->decrementReferenceCount();
    m_obstacles[index] = m_obstacles.back();
    m_obstacles.pop_back();
}

ysVector aw::Pathfinder::getLocation(int encodedIndex) const {
    int tx, ty;
    decode(encodedIndex, tx, ty);
    return getLocation(tx, ty);
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

bool aw::Pathfinder::isInRange(GameObject *obstacle) {
    int bounds = obstacle->getPathfinderBoundCount();
    for (int i = 0; i < bounds; ++i) {
        int minX, minY;
        int maxX, maxY;

        ysVector minPoint = ysMath::Sub(
            obstacle->getPathfinderBound(i).minPoint,
            ysMath::LoadVector(2.5f / 2, 2.5f / 2));
        ysVector maxPoint = ysMath::Add(
            obstacle->getPathfinderBound(i).maxPoint,
            ysMath::LoadVector(2.5f / 2, 2.5f / 2));

        translate(minPoint, minX, minY, false);
        translate(maxPoint, maxX, maxY, false);

        if (minX >= m_gridWidth) continue;
        if (maxX < 0) continue;
        if (minY >= m_gridWidth) continue;
        if (maxY < 0) continue;

        return true;
    }

    return false;
}

bool aw::Pathfinder::addObstacle(GameObject *obstacle) {
    if (findObstacle(obstacle)) return false;
    if (!isInRange(obstacle)) return false;

    obstacle->incrementReferenceCount();
    m_obstacles.push_back(obstacle);

    return true;
}

void aw::Pathfinder::clearObstacles() {
    for (GameObject *obstacle : m_obstacles) {
        obstacle->decrementReferenceCount();
    }

    m_obstacles.clear();
}

void aw::Pathfinder::refreshGrid() {
    for (int i = 0; i < m_gridWidth; ++i) {
        for (int j = 0; j < m_gridWidth; ++j) {
            m_grid[j][i] = false;
        }
    }

    int N = getObstacleCount();
    for (int i = 0; i < N; ++i) {
        GameObject *object = m_obstacles[i];
        if (object->getDeletionFlag()) {
            removeObstacle(i);
            --i; --N;
            continue;
        }

        bool inRange = false;
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

            inRange = true;

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

        if (!inRange) {
            removeObstacle(i);
            --i; --N;
        }
    }
}

bool aw::Pathfinder::update(const ysVector &currentLocation, bool collisionDetected, bool newObstacle, ysVector *nextTarget) {
    refreshGrid();

    int lx, ly;
    translate(currentLocation, lx, ly);

    bool evasiveAction = m_grid[ly][lx] || collisionDetected;

    if (!evasiveAction && (m_pathIndex == getPathLength() || newObstacle)) {
        ysVector delta = ysMath::Sub(currentLocation, m_destination);
        float mag = ysMath::GetScalar(ysMath::MagnitudeSquared3(delta));

        if (mag > 0.01f) {
            clearPath();

            setOrigin(currentLocation);
            refreshGrid();
            pathfind(m_destination, m_path);
        }
    }

    if (evasiveAction) {
        clearPath();

        setOrigin(currentLocation);
        refreshGrid();
        evade(m_path);

        m_evading = true;
    }

    if (m_pathIndex < getPathLength()) {
        int target = m_path[m_pathIndex];
        int current;
        int cx, cy;
        translate(currentLocation, cx, cy);
        current = encode(cx, cy);

        if (current == target) {
            ++m_pathIndex;
        }
    }

    if (m_pathIndex < getPathLength()) {
        *nextTarget = getLocation(m_path[m_pathIndex]);
        return true;
    }
    else {
        m_evading = false;

        clearPath();
        return false;
    }
}

int aw::clamp(int s, int l, int r) {
    if (s >= r) return r;
    else if (s <= l) return l;
    else return s;
}
