#ifndef __CYAN_MISCELIUM_GRAPH__
#define __CYAN_MISCELIUM_GRAPH__

#include "cm.h"

namespace CyanMycelium
{

    typedef Node * NodePtr ;
    typedef Property * PropertyPtr ;
    typedef MetaData * MetaDataPtr ;

    struct Property {
        char * Key;
        void * Value;
    };

    class MetaData {
        public:
        cm_size_t Count();
        void * Get(const char * key);
        bool TrySet(const char * key, void * value);
        bool TryRemove(const char * key);

        private:
        uint8_t _propertyCount;
        Property _properties[GRAPH_METADATA_SIZE];
    };

    class Node {
        public:
        MetaDataPtr Infos; 
        const char * Name;
        void * Value;

        struct{
            uint8_t in;
            uint8_t out;
        } Count;

        NodePtr inputs[ GRAPH_INPUT_SIZE + 1];
        NodePtr outputs[ GRAPH_OUTPUT_SIZE + 1];

        virtual bool Activate(void * context) = 0;
    };

    class Graph : Node {
    };
}

#endif