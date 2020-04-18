#include <cmath>
#include <vector>
#include <set>
#include "node.h"
#include "./../CppReflection/CppReflection.h"

int main()
{
   auto& clrMgr = NodeColorMgr::getGlobal();

   //Create 100 Nodes
   std::vector<Node*> randomNodes;
   auto totalColor = (unsigned short)Node::Color::NumItems;
   Node::Color c;
   for(int i = 0; i < 100; ++i)
   {
      Node::ushort_to_enum(i%totalColor,c);
      randomNodes.push_back(new Node(i,c));
   }

   std::cout  <<   "\n\n1. Initial distribution."   <<   std::endl;
   clrMgr.printStateUsage(__LINE__);
   std::cout << CppReflect(clrMgr.getStateUsage(Node::Color::BLACK));
   std::cout << CppReflect(randomNodes.back()->getColor());

   clrMgr.migrateGivenStateTo(Node::Color::WHITE, Node::Color::BLACK);
   std::cout << "\n\n2. After  migrating  all  WHITE  nodes  to  BLACK"  <<  std::endl;
   clrMgr.printStateUsage(__LINE__);
   std::cout << CppReflect(clrMgr.getStateUsage(Node::Color::BLACK));
   std::cout << CppReflect(randomNodes.back()->getColor());

   clrMgr.swapStates(Node::Color::GRAY, Node::Color::BLACK);
   std::cout  <<  "\n\n 3. After  swapping  GRAY  and   BLACK   nodes"   <<   std::endl;
   clrMgr.printStateUsage(__LINE__);
   std::cout << CppReflect(clrMgr.getStateUsage(Node::Color::BLACK));
   std::cout << CppReflect(randomNodes.back()->getColor());

   for(int i = 0; i < std::min((int)20,(int)randomNodes.size()); ++i)
   {
      if(randomNodes[i])
      {
         delete randomNodes[i];
         randomNodes[i] = nullptr;
      }
   }

   std::cout  <<  "\n\n 4. After deleting first 20 nodes"   <<   std::endl;
   clrMgr.printStateUsage(__LINE__);
   std::cout << CppReflect(clrMgr.getStateUsage(Node::Color::BLACK));
   std::cout << CppReflect(randomNodes.back()->getColor());

   clrMgr.migrateAllStatesTo(Node::Color::WHITE);
   std::cout  <<  "\n\n 5. After migrating all nodes to WHITE"   <<   std::endl;
   clrMgr.printStateUsage(__LINE__);
   std::cout << CppReflect(clrMgr.getStateUsage(Node::Color::BLACK));
   std::cout << CppReflect(randomNodes.back()->getColor());

   std::cout  <<  "\n\n 6. After setting last node as RED"   <<   std::endl;
   randomNodes.back()->setColor(Node::Color::RED);
   clrMgr.printStateUsage(__LINE__);
   std::cout << CppReflect(clrMgr.getStateUsage(Node::Color::BLACK));
   std::cout << CppReflect(randomNodes.back()->getColor());

   std::cout  <<  "\n\n 7. Let's create lot of fragments in WHITE to trigger auto defragmentation"   <<   std::endl;
   std::vector<Node*> whiteNodes;
   float attempt = 7.0;
   bool constTime = true;
   while(attempt < 8 && constTime == true)
   {
      attempt += 0.01;
      std::cout << "Randomization Attempt - " << attempt  << std::endl;
      clrMgr.getStateNodes(Node::Color::WHITE, whiteNodes);
      const auto whiteCount = whiteNodes.size();
      if(whiteCount > 0)
      {
         // take random white cells and change them to other colors.
         auto randIndex = [=](){return (rand()%whiteCount);};
         constTime = clrMgr.setState(whiteNodes.at(randIndex()),Node::Color::RED) && constTime;
         constTime = clrMgr.setState(whiteNodes.at(randIndex()),Node::Color::BLACK) && constTime;
         constTime = clrMgr.setState(whiteNodes.at(randIndex()),Node::Color::GRAY) && constTime;
      }
      constTime = clrMgr.migrateAllStatesTo(Node::Color::WHITE) && constTime;
      if(constTime == false)
         std::cout << "Note: Defragmentation happened to maintain amortized const time." << std::endl;
      clrMgr.printStateUsage(__LINE__);
   }

   // Clear local memory
   for(auto r : randomNodes)
   {
      if(r)
         delete r;
   }
   return 0;
}
