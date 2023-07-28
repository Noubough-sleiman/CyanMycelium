#include "cm_graph_utils.hpp"

using  namespace CyanMycelium ;         

  template<typename T>
  T KeyValueCollection<T> :: operator [] ( const char * name)
  {
    if(this->Count)
    {
      for(int i=0; i != this->Count; ++i)
      {
          T * current  = this->Items + i;
          if( strcmp(name,current->Key) == 0)
          {
              return current->Value;
          }
      }
    }
    return nullptr;
  }

