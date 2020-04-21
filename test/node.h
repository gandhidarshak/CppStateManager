
// #if COMPILING_Node
//     #define DLL_EXIMPORT __declspec(dllexport)
// #else
//     #define DLL_EXIMPORT __declspec(dllimport)
// #endif

#include "./../src/CppStateManager.h"
#include "./../CppEnumCreator/src/CppEnumCreatorIncludes.h"

// template class DLL_EXIMPORT CppStateManager<Node, Node::Color>;

class Node
{
   public:

      // I am using CppEnumCreator code to create an enum but you can use any
      // other enum as well. Kepp NumItems as the last element in your enum.
#define CppEnumParentClass Node
#define CppEnumName Color
#define CppEnumList WHITE, RED, GRAY, BLACK
#include "./../CppEnumCreator/src/CppEnumCreatorDeclaration.h"

      int getVal(){return val;};
      void setVal(const int i){val=i;};
      Node::Color getColor();
      void setColor(const Node::Color c);
      void clearColor();
      Node(int v, Node::Color c);
      ~Node(){clearColor();};

   private:
      int val;
      // Node class should have below lines.
      friend class CppStateManager<Node, Node::Color>;
      friend class CppStateNodesBundle<Node, Node::Color>;
      const CppStateNodesBundle<Node, Node::Color>* mp_state;
};


class NodeColorMgr : public CppStateManager<Node, Node::Color> {
   // Define getGlobal if you want to extend the base class.
   // public: 
   //    static NodeColorMgr& getGlobal();
   //    ...
};

