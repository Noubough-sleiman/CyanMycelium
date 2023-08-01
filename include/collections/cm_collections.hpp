#ifndef __CYAN_MISCELIUM_COLLECTIONS__
#define __CYAN_MISCELIUM_COLLECTIONS__

namespace CyanMycelium
{
  template<typename T>
  class  Collection 
  {
    public:
      class Iterator
      {
        public:
          Iterator(Collection<T> * src)
          {
            _src = src ;
            Reset();
          }

          boolean MoveNext();
          virtual T * Current();
          void Reset();

        protected:
          int _i;
          Collection<T> * _src;
      };

      Collection(unsigned int initialCapacity = 2);
      ~Collection();
      unsigned int Count();
      T operator[](unsigned int i) { return _items[i]; }
      void Add(T);
      void Trim();
      virtual bool Contains(T);
      Iterator GetIterator(){ return Iterator(this); }

    protected:
      void EnsureEnoughRoomFor(unsigned int);
      void SetCapacity(unsigned int);

      T * _items;
      unsigned int _count;
      unsigned int _capacity;
  };

  #define KEY_MAX_LENGTH 32

  template<typename T>
  struct KeyValue 
  {
    const char Key[KEY_MAX_LENGTH];
    T Value;
  };

  template<typename T>
  class KeyValueCollection : public Collection<KeyValue<T>>
  {
    public:
      using Collection<KeyValue<T>>::operator[]; // Bring the base class's operator[] into scope
      T operator[](const char * name);
      T Get(const char * name);
      void Set(const char * name, T value);
  };

}
#endif