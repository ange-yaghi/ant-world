#ifndef DELTA_TEMPLATE_TEMPLATE_APPLICATION_H
#define DELTA_TEMPLATE_TEMPLATE_APPLICATION_H

#include "delta.h"

#include "world.h"

namespace aw {

    class AntWorld {
    public:
        AntWorld();
        ~AntWorld();

        void Initialize(void *instance, ysContextObject::DEVICE_API api);
        void Run();

    protected:
        World world;
    };

} /* namespace aw */

#endif /* DELTA_TEMPLATE_TEMPLATE_APPLICATION_H */
