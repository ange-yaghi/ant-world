#ifndef YDS_TRANSFORM_H
#define YDS_TRANSFORM_H

#include "yds_math.h"

class ysTransform {
public:
    ysTransform();
    ~ysTransform();

    ysQuaternion GetLocalOrientation() const;
    ysQuaternion GetWorldOrientation() const;

    ysVector GetLocalPosition() const;
    ysVector GetWorldPosition() const;

    void SetParent(ysTransform *parent);
    ysTransform *GetParent() const { return m_parent; }

private:
    ysVector m_position;
    ysQuaternion m_orientation;

    ysTransform *m_parent;
};

#endif /* YDS_TRANSFORM_H */
