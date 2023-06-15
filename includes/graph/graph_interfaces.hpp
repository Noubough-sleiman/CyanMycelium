#ifndef __CYAN_MISCELIUM_GRAPH_INTERFACES__
#define __CYAN_MISCELIUM_GRAPH_INTERFACES__

namespace CyanMycelium         
{
  class Node 
  {
    public:
      /**
       *  All the outgoing edges
       */
      virtual int onsc(Link ** target, int * size) = 0;
      /**
       *  All the incoming edges
       */       
      virtual int opsc(Link ** target, int * size) = 0;
    private:
  };

  class Link 
  {
    public:
      /**
       * Initial Node. 
       */
      Node * initialNode;
      /**
       * Final Node. 
       */      
      Node * finalNode ;
     
    private:
  };

  class Graph
  {
    public:
       /**
        *  Nodes with no incoming edges
        */
       virtual int entryNodes(Node ** target, int * size) = 0;
       /**
        *  Nodes with no outgoing edges.
        */
       virtual int exitNodes(Node ** target, int * size) = 0;
    private:
  };
}

#endif