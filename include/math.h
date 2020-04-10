#ifndef ANT_WORLD_MATH_H
#define ANT_WORLD_MATH_H

#include "delta.h"

namespace aw {

    ysVector vector3d(const ysVector &b);
    float distance(const ysVector &a, const ysVector &b);
    float length(const ysVector &v);
    float length2(const ysVector &v);
    bool inRange(const ysVector &a, const ysVector &b, float distance);
    bool inRangeSq(const ysVector &a, const ysVector &b, float distance2);
    bool getDirection(const ysVector &pos, const ysVector &target, ysVector *out);

} /* namespace aw */

#endif /* ANT_WORLD_MATH_H */
