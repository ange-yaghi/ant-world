#ifndef ANT_WORLD_INTERIOR_GENERATION_H
#define ANT_WORLD_INTERIOR_GENERATION_H

#include "delta.h"

namespace aw {

    class Realm;

    class InteriorGenerator {
    public:
        struct WallsParam {
            float GridSize;
            int MaxCells;
            int MinCells;
        };

    public:
        InteriorGenerator();
        ~InteriorGenerator();

        static void generateWalls(Realm *realm, const WallsParam &param);
        static void generateRectangle(Realm *realm, float width, float height);
        static void generateWall(Realm *realm, const ysVector &center, float thickness, float length, float unitSize, bool vertical);
    };

} /* namespace aw */

#endif /* ANT_WORLD_INTERIOR_GENERATION_H */
