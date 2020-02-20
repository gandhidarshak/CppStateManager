#include "test.h"

#include "./../src/CppStateManagerDefinition.h"
#include "./../CppReflection/CppReflection.h"

int main()
{
   auto& sm = CppStateManager::getGlobal();

   CppStateManager::ConstStatePtr cell1State, cell2State, cell3State, cell4State;
   cell1State = sm.getStatePtr("WHITE");
   cell2State = sm.getStatePtr("WHITE");
   cell3State = sm.getStatePtr("WHITE");
   cell4State = sm.getStatePtr("WHITE");
   std::cout << "After setting everything as WHITE" << std::endl;
   std::cout << CppReflectAsCSV(*cell1State,*cell2State,*cell3State,*cell4State) << std::endl;
   sm.printStateUsage(__LINE__);
   std::cout << CppReflectAsList(sm.getStateUsage("WHITE")) << std::endl;

   cell2State = sm.getStatePtr(CppStateManager::States::GRAY);
   cell3State = sm.getStatePtr(CppStateManager::States::BLACK);
   std::cout << "After making cell2 as GRAY and cell3 as BLACK" << std::endl;
   std::cout << CppReflectAsCSV(*cell1State,*cell2State,*cell3State,*cell4State) << std::endl;
   sm.printStateUsage(__LINE__);
   std::cout << CppReflectAsList(sm.getStateUsage("WHITE")) << std::endl;

   sm.migrateState("WHITE", "BLACK");
   std::cout << "After migrating all WHITE cells to BLACK" << std::endl;
   std::cout << CppReflectAsCSV(*cell1State,*cell2State,*cell3State,*cell4State) << std::endl;
   sm.printStateUsage(__LINE__);
   std::cout << CppReflectAsList(sm.getStateUsage("WHITE")) << std::endl;

   sm.swapStates("GRAY", "BLACK");
   std::cout << "After swapping GRAY and BLACK Cells" << std::endl;
   std::cout << CppReflectAsCSV(*cell1State,*cell2State,*cell3State,*cell4State) << std::endl;
   sm.printStateUsage(__LINE__);
   std::cout << CppReflectAsList(sm.getStateUsage("WHITE")) << std::endl;

   return 0;
}
