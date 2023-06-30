#include "../includes/cm_graph.hpp"

int index_of_s( CyanMycelium::PropertyPtr properties, const char * key, cm_size_t size);

cm_size_t CyanMycelium::MetaData::Count()
{
    return this->_propertyCount;
}

void * CyanMycelium::MetaData::Get(const char * key)
{
    int i = index_of_s(this->_properties, key, this->_propertyCount);
    if( i >= 0 )
    {
        return this->_properties[i].Value;
    }
    return NULL;
}

bool CyanMycelium::MetaData::TrySet(const char * key, void * value)
{
    if( key == NULL || this->_propertyCount == GRAPH_METADATA_SIZE) return false;
    PropertyPtr p = this->_properties + this->_propertyCount ;
    p->Key = key;
    p->Value = value; 
    this->_propertyCount++;
    return true;
}

bool CyanMycelium::MetaData::TryRemove(const char * key)
{
    int i = index_of_s(this->_properties, key, this->_propertyCount);
    if( i >= 0 )
    {
      PropertyPtr a = this->_properties;
      if( i < this->_propertyCount -1 )
      {
        a += i;
        size_t size = (this->_propertyCount-i-1) * sizeof(Property);
        cm_memcpy(a, a+1, size );
      }
      this->_propertyCount --;
      a = this->_properties + this->_propertyCount ;
      a->Key = NULL;
      a->Value = NULL; 
      return true;
    }
    return false;
}

int index_of_s( CyanMycelium::PropertyPtr properties, const char * key, cm_size_t size)
{
  if( key == NULL || size == 0 ) return -1;   

  for(cm_size_t i=0; i != size; i++, properties++)
  {
    if( properties->Key && cm_strcmp(properties->Key, key) == 0)
    {
      return i;
    }
  }
  return -1;
}