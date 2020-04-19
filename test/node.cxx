
#include "node.h"

#define CppEnumParentClass Node
#define CppEnumName Color 
#include "./../CppEnumCreator/src/CppEnumCreatorDefinition.h" 

Node::Node(int v, Node::Color c) 
{
   setVal(v);
   mp_state = nullptr;
   setColor(c);
}

Node::Color Node::getColor()
{
   if(mp_state)
   {
      auto& ncm = NodeColorMgr::getGlobal();
      return ncm.getState(this);
   }
   return Node::Color::NumItems;
}

void Node::setColor(const Node::Color c)
{
   auto& ncm = NodeColorMgr::getGlobal();
   ncm.setState(this,c);
}

void Node::clearColor()
{
   if(mp_state)
   {
      auto& ncm = NodeColorMgr::getGlobal();
      ncm.clearState(this);
   }
}


// Define getGlobal if you want to extend the base class.
// NodeColorMgr& NodeColorMgr::getGlobal()
// {
//    static NodeColorMgr singleton;
//    return singleton;
// }


