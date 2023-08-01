#include "cm.h"
#include "collections/cm_collections.hpp"

using  namespace CyanMycelium ;    


template<typename T>
boolean Collection<T>::Iterator :: MoveNext()
{
  unsigned int c = _src->Count();
  if( _i < c)
  {
    _i++;
  }
  return _i < c;
}

template<typename T>
T* Collection<T>::Iterator :: Current()
{
  return _i >= 0 ? _src->_items+_i : nullptr;
}

template<typename T>
void Collection<T>::Iterator :: Reset()
{
  _i = -1;
}


template<typename T>
Collection<T> :: Collection(unsigned int initialCapacity)
{
  _items = nullptr;
  SetCapacity(max(initialCapacity,2));
  _count = 0;
}

template<typename T>
Collection<T> :: ~Collection()
{
  if(_items)
  {
    free(_items);
  }
}


template<typename T>
void Collection<T> :: Add(T obj)
{
  EnsureEnoughRoomFor(1);
  if(_items)
  {
    _items[_count++] = obj ;
  }
}

template<typename T>
bool Collection<T> :: Contains(T obj)
{
  if( _count == 0 )
  {
    return false;
  }
  for(int i=0; i < _count ; ++i)
  {
    if( _items[i] == obj )
    {
      return true;
    }
  }
  return false;
}

template<typename T>
void Collection<T> :: Trim()
{
  if(_count != _capacity)
  {
    SetCapacity(_count);
  }
}

template<typename T>
void Collection<T> :: EnsureEnoughRoomFor (unsigned int n)
{
  unsigned int targetCount = _count + n ;
  unsigned int targetCapacity = _capacity;
  if( targetCount >= targetCapacity)
  {
    do
    {
      targetCapacity += max(targetCapacity/3, 2);
    } while (targetCount >= targetCapacity);
    SetCapacity(targetCapacity);
  }
}

template<typename T>
void Collection<T> :: SetCapacity(unsigned int newCapacity)
{
  unsigned int targetCapacity = max(newCapacity, Count());
  _items = realloc(_items, targetCapacity * sizeof(T) );
  _capacity = _items ? newCapacity : 0;
}

template<typename T>
T KeyValueCollection<T> :: operator [] ( const char * key)
{
  return Get(key);
}

template<typename T>
T KeyValueCollection<T> :: Get ( const char * key)
{
  if(this->_count)
  {
    for(int i=0; i != this->_count; ++i)
    {
        KeyValue<T> * current  = this->_items + i;
        if( strcmp(key,current->Key) == 0)
        {
            return current->Value;
        }
    }
  }
  return nullptr;
}

template<typename T>
void KeyValueCollection<T> :: Set ( const char * key, T value)
{
  if(this->_count)
  {
    for(int i=0; i != this->_count; ++i)
    {
        KeyValue<T> * current  = this->_items + i;
        if( strcmp(key,current->Key) == 0)
        {
            current->Value = value;
            return;
        }
    }
  }
  if( this->_count == this->_capacity)
  {
    unsigned int c = this->_capacity + max(this->_capacity/3, 2);
    this->SetCapacity(c);
  }
  KeyValue<T> * entry  = this->_items + this->_count;
  strcpy_s(entry->Key, KEY_MAX_LENGTH, key);
  entry->Value = value;
  this->_count++;
}
