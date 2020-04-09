#ifndef ANT_WORLD_FLOORING_H
#define ANT_WORLD_FLOORING_H

#include "game_object.h"

namespace aw {

    class Flooring : public GameObject {
    public:
        Flooring();
        ~Flooring();

        virtual void initialize();
        virtual void process();
        virtual void render();

        void setDimensions(float width, float height) { m_width = width; m_height = height; }
        float getWidth() const { return m_width; }
        float getHeight() const { return m_height; }

    protected:
        float m_width;
        float m_height;
    };

} /* namespace aw */

#endif /* ANT_WORLD_FLOORING_H */
