

#ifndef CppStateManager 
#define CppStateManager CppStateManager
#endif


// Create Enum related APIs.
#define CppEnumName States
#define CppEnumParentClass CppStateManager
#include "./../CppEnumCreator/src/CppEnumCreatorDefinition.h"
      
CppStateManager& CppStateManager::getGlobal()
{
   static CppStateManager singleton;
   return singleton;
}

CppStateManager::CppStateManager()
{
   m_states.clear();
}

CppStateManager::~CppStateManager()
{
   m_states.clear();
}

CppStateManager::ConstStatePtr CppStateManager::getStatePtr(std::string sStr)
{
   States s;
   CppStateManager::string_to_enum(sStr, s);
   return getStatePtr(s);
}

CppStateManager::ConstStatePtr CppStateManager::getStatePtr(States s)
{
   auto itr = m_states.find(s);
   if(itr == m_states.end())
   {
      itr = m_states.insert(std::make_pair(s,StatePtrList())).first;
   }
   if(itr->second.empty())
   {
      StatePtr sp(new States(s));
      itr->second.push_front(sp);
   }
   return itr->second.front();
}

void CppStateManager::migrateState(std::string fromStr, std::string toStr)
{
   States from, to;
   CppStateManager::string_to_enum(fromStr, from);
   CppStateManager::string_to_enum(toStr, to);
   return migrateState(from, to);
}

void CppStateManager::migrateState(States from, States to)
{
   auto itrFrom = m_states.find(from);
   if(itrFrom == m_states.end())
      return;

   auto itrTo = m_states.find(to);
   if(itrTo == m_states.end())
      itrTo = m_states.insert(std::make_pair(to,StatePtrList())).first;

   auto& fromList = itrFrom->second;
   auto& toList = itrTo->second;
   toList.splice_after(toList.cbefore_begin(), fromList);
   fromList.clear();
   purgeStateUsage();

}

void CppStateManager::swapStates(std::string oneStr, std::string twoStr)
{
   States one, two;
   CppStateManager::string_to_enum(oneStr, one);
   CppStateManager::string_to_enum(twoStr, two);
   return swapStates(one, two);
}

void CppStateManager::swapStates(States one, States two)
{
   if(one==two)
      return;
   if(one == States::NumItems || two == States::NumItems)
   {
      std::cerr << "Can not swap states with state NumItems as its just a place-holder" << std::endl;
      return;
   }
   migrateState(one,States::NumItems);
   migrateState(two,one);
   migrateState(States::NumItems,two);
}

void CppStateManager::purgeStateUsage()
{
   for(auto& mapElem : m_states)
   {
      States s = mapElem.first;
      StatePtrList sList = mapElem.second;
      auto itrBefore = sList.before_begin();
      auto itrNext = sList.begin();
      while(itrNext != sList.end())
      {
         unsigned int useCount = itrNext->use_count();
         --useCount; // To discard usage of pointer in m_states.
         --useCount; // To discard usage of pointer in the iter
         if(useCount <= 0)
         {
            itrNext = sList.erase_after(itrBefore);
            continue;
         }
         (*(*itrNext)) = s; // make sure that map with index s has all values as s
         ++itrBefore;
         ++itrNext;
      }
   }
}

void CppStateManager::printStateUsage(unsigned int lineNo) 
{
   std::cout << "CppStateManager State Usage";
   if(lineNo>0)
      std::cout << " on line "<< lineNo ;
   std::cout << ":" << std::endl;
   unsigned int totalCounter = 0;
   for(auto& mapElem : m_states)
   {
      unsigned int counter = 0;
      States s = mapElem.first;
      StatePtrList sList = mapElem.second;
      auto itrBefore = sList.before_begin();
      auto itrNext = sList.begin();
      while(itrNext != sList.end())
      {
         unsigned int useCount = itrNext->use_count();
         --useCount; 
         --useCount; 
         if(useCount <= 0)
         {
            itrNext = sList.erase_after(itrBefore);
            continue;
         }
         counter += useCount;
         if((*(*itrNext)) != s) // make sure that map with index s has all values as s
            std::cerr << (*(*itrNext)) << " state found instead of state " << s << std::endl;
         ++itrBefore;
         ++itrNext;
      }
      if(counter>0)
      {
         std::cout << "\t" << mapElem.first << " -> " << counter << std::endl;
         totalCounter += counter;
      }
   }
   std::cout << "\t" << "Total" << " -> " << totalCounter << std::endl << std::endl;
}

#undef CppStateManager 
