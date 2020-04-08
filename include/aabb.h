#ifndef ANT_WORLD_AABB_H
#define ANT_WORLD_AABB_H

#include "delta.h"

namespace aw {

    struct AABB {
        ysVector minPoint;
        ysVector maxPoint;

        bool intersects2d(const AABB &aabb) const {
            if (ysMath::GetX(maxPoint) < ysMath::GetX(aabb.minPoint)) return false;
            if (ysMath::GetX(minPoint) > ysMath::GetX(aabb.maxPoint)) return false;

            if (ysMath::GetY(maxPoint) < ysMath::GetY(aabb.minPoint)) return false;
            if (ysMath::GetY(minPoint) > ysMath::GetY(aabb.maxPoint)) return false;

            return true;
        }
    };

} /* namespace aw */

#endif /* ANT_WORLD_AABB_H */
