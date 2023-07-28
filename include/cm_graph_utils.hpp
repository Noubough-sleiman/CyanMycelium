#ifndef __CYAN_MISCELIUM_GRAPH_UTILS__
#define __CYAN_MISCELIUM_GRAPH_UTILS__

#include "cm.h"

namespace CyanMycelium
{
    /// @brief 
    /// @tparam T 
    template<typename T>
    class  Collection {
      public:
      uint16_t Count;
      T * Items;
      T operator[](int i)
      {
        return Items[i];
      }
    };

    #define KEY_MAX_LENGTH 16

    /// @brief 
    /// @tparam T 
    template<typename T>
    struct KeyValue {
      const char Key[KEY_MAX_LENGTH];
      T Value;
    };

    /// @brief 
    /// @tparam T 
    template<typename T>
    struct KeyValueCollection : public Collection<KeyValue<T>>
    {
        using Collection::operator[]; // Bring the base class's operator[] into scope
        T operator[](const char * name);
    };
}
#endif