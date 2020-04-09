#ifndef ANT_WORLD_INTERIOR_GENERATION_H
#define ANT_WORLD_INTERIOR_GENERATION_H

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
    };

} /* namespace aw */

#endif /* ANT_WORLD_INTERIOR_GENERATION_H */
