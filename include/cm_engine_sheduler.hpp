#ifndef __CYAN_MISCELIUM_ENGINE_SCHEDULER__
#define __CYAN_MISCELIUM_ENGINE_SCHEDULER__

#include "cm_engine.hpp"
#include "concurrent/cm_concurrent_queue.hpp"

namespace CyanMycelium         
{
    #define CM_DEFAULT_CORE_COUNT 2
    struct ActivationItem
    {
        InferenceSessionPtr Session;
        NodePtr Node;
    };
    typedef ActivationItem * ActivationItemPtr;

    class NodeActivationScheduler : ConcurrentQueue<ActivationItemPtr>
    {
        public:
        NodeActivationScheduler(uint8_t nthread = CM_DEFAULT_CORE_COUNT);

        void Consume(ActivationItemPtr a) override;
    }
}
#endif