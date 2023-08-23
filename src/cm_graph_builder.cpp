#include "cm_graph_builder.hpp"

using namespace CyanMycelium;

GraphBuilder &GraphBuilder ::WithNode(const char *k, NodePtr v)
{
  if (k && v)
  {
    _nodes.Set(k, v);
  }
  return *this;
}

GraphBuilder &GraphBuilder ::WithLink(const char *k, LinkPtr v)
{
  if (k && v)
  {
    _links.Set(k, v);
  }
  return *this;
}

GraphBuilder &GraphBuilder ::WithConnection(NodePtr ini, LinkPtr l, NodePtr fin)
{
  l->Oini = ini;
  l->Ofin = fin;
  if (ini)
  {
    ini->Onsc.Add(l);
  }
  if (fin)
  {
    fin->Opsc.Add(l);
  }
  return *this;
}

GraphBuilder &GraphBuilder ::WithConnection(LinkPtr p, NodePtr o, LinkPtr n)
{
  if (p && !o->Opsc.Contains(p))
  {
    o->Opsc.Add(p);
    p->Ofin = o;
  }
  if (n && !o->Onsc.Contains(n))
  {
    o->Onsc.Add(n);
    n->Oini = o;
  }
  return *this;
}

NodePtr GraphBuilder ::GetNode(const char *k)
{
  return _nodes[k];
}

LinkPtr GraphBuilder ::GetLink(const char *k)
{
  return _links[k];
}

GraphPtr GraphBuilder ::Build()
{
  GraphPtr g = new Graph();

  KeyValueCollection<NodePtr>::Iterator<KeyValue<NodePtr>> i = _nodes.GetIterator();
  while (i.MoveNext())
  {
    KeyValue<NodePtr> *entry = i.Current();
    if (entry && entry->Value)
    {
      g->Nodes.Add(entry->Value);
    }
  }

  KeyValueCollection<LinkPtr>::Iterator<KeyValue<LinkPtr>> j = _links.GetIterator();
  while (j.MoveNext())
  {
    KeyValue<LinkPtr> *entry = j.Current();
    if (entry && entry->Value && (entry->Value->Oini || entry->Value->Ofin))
    {
      g->Links.Add(entry->Value);
      if (entry->Value->Oini == nullptr)
      {
        g->inputs.Set(entry->Key, entry->Value);
      }
      if (entry->Value->Ofin == nullptr)
      {
        g->outputs.Set(entry->Key, entry->Value);
      }
    }
  }
  return g;
}
