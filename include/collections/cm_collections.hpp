#ifndef __CM_COLLECTIONS__
#define __CM_COLLECTIONS__

#include "cm.h"

namespace CyanMycelium
{
#ifndef CM_DEFAULT_COLLECTION_CAPACITY
#define CM_DEFAULT_COLLECTION_CAPACITY 2
#endif

  template <typename T>
  class Collection
  {
  public:
    template <typename V>
    class Iterator
    {
    public:
      Iterator(Collection<T> *src)
      {
        _src = src;
        Reset();
      };

      virtual boolean MoveNext()
      {
        int c = _src->Count();
        if (_i < c)
        {
          _i++;
        }
        return _i < c;
      };

      virtual V *Current()
      {
        return _i >= 0 ? _src->_items + _i : nullptr;
      };

      void Reset()
      {
        _i = -1;
      };

      void To(Collection<V> *c)
      {
        Reset();
        if (MoveNext())
        {
          do
          {
            V *v = Current();
            if (v)
            {
              c->Add(*v);
            }
          } while (MoveNext());
        }
      }

      void ToSingle(Collection<V> *c)
      {
        Reset();
        if (MoveNext())
        {
          do
          {
            V *v = Current();
            if (c->Contains(*v))
            {
              continue;
            }
            c->Add(*v);
          } while (MoveNext());
        }
      }

    protected:
      int _i;
      Collection<T> *_src;
    };

    Collection(unsigned int initialCapacity = CM_DEFAULT_COLLECTION_CAPACITY)
    {
      _items = nullptr;
      SetCapacity(max(initialCapacity, CM_DEFAULT_COLLECTION_CAPACITY));
      _count = 0;
    };

    ~Collection()
    {
      if (_items)
      {
        free(_items);
      }
    };

    int Count() { return _count; };

    T &operator[](int i) { return _items[i]; };

    T &First() { return _items[0]; }

    T &Last() { return _items[_count - 1]; }

    Collection<T> &Add(T &obj)
    {
      EnsureEnoughRoomFor(1);
      if (_items)
      {
        _items[_count++] = obj; // copy
      }
      return *this;
    };

    Collection<T> &Add(Collection<T> &other)
    {
      other.GetIterator().To(this);
      return this;
    };

    Collection<T> &Add(Iterator<T> &i)
    {
      i.To(this);
      return *this;
    };

    Collection<T> &Add()
    {
      EnsureEnoughRoomFor(1);
      _count++;
      return *this;
    };

    Collection<T> &Trim()
    {
      if (_count != _capacity)
      {
        SetCapacity(_count);
      }
      return *this;
    };

    bool Contains(T &obj)
    {
      if (_count == 0)
      {
        return false;
      }
      for (int i = 0; i < _count; ++i)
      {
        if (_items[i] == obj)
        {
          return true;
        }
      }
      return false;
    };

    Iterator<T> GetIterator() { return Iterator<T>(this); };

    void EnsureEnoughRoomFor(int n)
    {
      int targetCount = _count + n;
      int targetCapacity = _capacity;
      if (targetCount >= targetCapacity)
      {
        do
        {
          targetCapacity += max(targetCapacity / 3, 2);
        } while (targetCount >= targetCapacity);
        SetCapacity(targetCapacity);
      }
    };

    void SetCapacity(int newCapacity)
    {
      int targetCapacity = max(newCapacity, Count());
      _items = (T *)cm_realloc(_items, targetCapacity * sizeof(T));
      _capacity = _items ? newCapacity : 0;
    };

  protected:
    T *_items;
    int _count;
    int _capacity;
  };

#ifndef CM_KEY_MAX_LENGTH
#define CM_KEY_MAX_LENGTH 32
#endif

  template <typename T>
  struct KeyValue
  {
    char Key[CM_KEY_MAX_LENGTH];
    T Value;
  };

  template <typename T>
  class KeyValueCollection : public Collection<KeyValue<T>>
  {

  public:
    KeyValueCollection(unsigned int initialCapacity = 2) : Collection<KeyValue<T>>(initialCapacity)
    {
    }

    using Collection<KeyValue<T>>::operator[]; // Bring the base class's operator[] into scope
    T operator[](const char *key) { return Get(key); }
    T Get(const char *key)
    {
      if (this->_count)
      {
        for (int i = 0; i != this->_count; ++i)
        {
          KeyValue<T> *current = this->_items + i;
          if (strcmp(key, current->Key) == 0)
          {
            return current->Value;
          }
        }
      }
      return nullptr;
    };

    void Set(const char *key, T value)
    {
      if (this->_count)
      {
        for (int i = 0; i != this->_count; ++i)
        {
          KeyValue<T> *current = this->_items + i;
          if (strcmp(key, current->Key) == 0)
          {
            current->Value = value;
            return;
          }
        }
      }
      if (this->_count == this->_capacity)
      {
        unsigned int c = this->_capacity + max(this->_capacity / 3, 2);
        this->SetCapacity(c);
      }
      KeyValue<T> *entry = this->_items + this->_count;
      strcpy_s(entry->Key, CM_KEY_MAX_LENGTH, key);
      entry->Value = value;
      this->_count++;
    }
  };
}
#endif