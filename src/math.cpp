#include "../include/math.h"

ysVector aw::vector3d(const ysVector &b) {
    return ysMath::Mask(b, ysMath::Constants::MaskOffW);
}

float aw::distance(const ysVector &a, const ysVector &b) {
    ysVector d = ysMath::Sub(a, b);
    return ysMath::GetScalar(ysMath::Magnitude(vector3d(d)));
}

bool aw::inRange(const ysVector &a, const ysVector &b, float distance) {
    return inRangeSq(a, b, distance * distance);
}

bool aw::inRangeSq(const ysVector &a, const ysVector &b, float distance2) {
    ysVector d = ysMath::Sub(a, b);
    return ysMath::GetScalar(ysMath::MagnitudeSquared3(d)) <= distance2;
}

bool aw::getDirection(const ysVector &pos, const ysVector &target, ysVector *out) {
    ysVector d = ysMath::Sub(target, pos);
    ysVector mag2 = ysMath::MagnitudeSquared3(d);

    if (ysMath::GetScalar(mag2) <= 0.001f) return false;
    else {
        *out = vector3d(ysMath::Div(d, ysMath::Sqrt(mag2)));
        return true;
    }
}
