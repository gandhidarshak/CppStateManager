/*

MIT License

Copyright (c) 2020 Darshak Gandhi

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is furnished 
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/


#ifndef CppStateManager_h_
#define CppStateManager_h_

#include <vector>
#include <iterator>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <forward_list>

//------------------------------------------------------------------------------
// Assumption: It is assumed that Node class has a mp_state pointer that can  be
// directly accessed by StateManager class as a friend class or  public  member.
// This class is not thread-safe but if you need  a  TS  implementation,  please
// identify and put mutexes on m_states' usage.
//------------------------------------------------------------------------------

template<typename NODE_CLASS, typename STATE_ENUM> 
class CppStateNodesBundle
{
   public:
      STATE_ENUM state;
      std::unordered_set<NODE_CLASS*> nodes;
      CppStateNodesBundle(const STATE_ENUM s) { state = s; };
      ~CppStateNodesBundle();
      bool operator==(const STATE_ENUM to) const {return state == to;};
      bool operator!=(const STATE_ENUM to) const {return !(state == to);};
};

template<typename NODE_CLASS, typename STATE_ENUM>
CppStateNodesBundle<NODE_CLASS,STATE_ENUM>::~CppStateNodesBundle()
{
   //std::cout << "Destructor of CppStateNodesBundle called for state " << state << std::endl;
   for(auto n : nodes)
   {
      // State Manager doesn't own the cells so should not delete them
      if(n)
         n->mp_state = nullptr;
   }
}

template<typename NODE_CLASS, typename STATE_ENUM> 
class CppStateManager
{
   public:
   protected:
      //------------------------------------------------------------------------
      // Using singleton design pattern for CppStateManagerClass as protected.
      //------------------------------------------------------------------------
      CppStateManager( );
      virtual ~CppStateManager( );
      CppStateManager (const CppStateManager& obj ) = delete;
      CppStateManager& operator=( const CppStateManager& obj ) = delete;
   public:
      // For bool APIs, if the run-time was const 
      // (defragmentation didn't happen) then it will return true
      static CppStateManager& getGlobal();
      STATE_ENUM getState(const NODE_CLASS* n) const ;
      bool setState(NODE_CLASS* n, STATE_ENUM s);
      bool clearState(NODE_CLASS* n);
      bool purgeStateUsage(STATE_ENUM s);
      void printStateUsage(unsigned int lineNo=0);
      bool migrateGivenStateTo(STATE_ENUM from, STATE_ENUM to);
      bool migrateAllStatesTo(STATE_ENUM to);
      bool swapStates(STATE_ENUM one, STATE_ENUM two);
      unsigned int getStateUsage(STATE_ENUM s) const;
      void getStateNodes(STATE_ENUM s, std::vector<NODE_CLASS*>& nodes) const;
      void defragmentStateUsage(STATE_ENUM s);
   private:
      typedef CppStateNodesBundle<NODE_CLASS, STATE_ENUM>* StatePtr;
      typedef std::forward_list<StatePtr> StatePtrList;
      std::unordered_map<STATE_ENUM, StatePtrList> m_states;
};

template<typename NODE_CLASS, typename STATE_ENUM>
   CppStateManager<NODE_CLASS, STATE_ENUM>& 
CppStateManager<NODE_CLASS, STATE_ENUM>::getGlobal()
{
   static CppStateManager<NODE_CLASS, STATE_ENUM> singleton;
   return singleton;
}

   template<typename NODE_CLASS, typename STATE_ENUM>
CppStateManager<NODE_CLASS, STATE_ENUM>::CppStateManager()
{
   m_states.clear();
}

   template<typename NODE_CLASS, typename STATE_ENUM>
CppStateManager<NODE_CLASS, STATE_ENUM>::~CppStateManager()
{
   // std::cout << "Destructor of CppStateManager called" << std::endl;
   for(auto s : m_states)
   {
      for(auto p : s.second)
      {
         if(p)
            delete p;
      }
   }
   m_states.clear();
}

template<typename NODE_CLASS, typename STATE_ENUM>
STATE_ENUM 
CppStateManager<NODE_CLASS, STATE_ENUM>::getState(const NODE_CLASS* n) const
{
   if(n && n->mp_state)
      return n->mp_state->state;
   std::cerr << "Trying to getState for a node for which mp_state pointer is nullptr" << std::endl;
   return STATE_ENUM(); // will return 0th enum.
}

template<typename NODE_CLASS, typename STATE_ENUM>
   bool 
CppStateManager<NODE_CLASS, STATE_ENUM>::clearState(NODE_CLASS* n)
{
   bool constRunTime = true;
   if(!n)
      return constRunTime;
   auto p_nsb = const_cast<CppStateNodesBundle<NODE_CLASS, STATE_ENUM>*>(n->mp_state);
   if(!p_nsb)
      return constRunTime;
   auto erased = p_nsb->nodes.erase(n);
   if(erased == 0)
   {
      std::cerr << "Trying to clear state of a node which does not contain that state!" << std::endl;
      std::cerr << n->mp_state->state << " - " << p_nsb->state << std::endl;
   }

   // Avoid burning purging run-time as it's not mandatory 
   // if(p_nsb->nodes.size()==0)
   //    constRunTime = purgeStateUsage(p_nsb->state) && constRunTime ;

   n->mp_state = nullptr;
   return constRunTime;
}

template<typename NODE_CLASS, typename STATE_ENUM>
   bool 
CppStateManager<NODE_CLASS, STATE_ENUM>::setState(NODE_CLASS* n, STATE_ENUM s)
{
   bool constRunTime = true;
   if(!n)
      return constRunTime;
   // if node's state is same as s, do nothing, else clear it.
   if(n->mp_state)
   {
      if(*(n->mp_state) == s)
         return constRunTime;
      else
         constRunTime = clearState(n) && constRunTime;
   }
   auto itr = m_states.find(s);
   if(itr == m_states.end())
   {
      itr = m_states.insert(std::make_pair(s,StatePtrList())).first;
   }
   if(itr->second.empty())
   {
      StatePtr sp = new CppStateNodesBundle<NODE_CLASS, STATE_ENUM>(s);
      itr->second.push_front(sp);
   }
   itr->second.front()->nodes.insert(n);
   n->mp_state = itr->second.front();
   return constRunTime;
}

template<typename NODE_CLASS, typename STATE_ENUM>
   bool 
CppStateManager<NODE_CLASS, STATE_ENUM>::migrateGivenStateTo(STATE_ENUM from, STATE_ENUM to)
{
   bool constRunTime = true;
   if(from == to)
      return constRunTime;
   auto itrFrom = m_states.find(from);
   if(itrFrom == m_states.end())
      return constRunTime;

   auto itrTo = m_states.find(to);
   if(itrTo == m_states.end())
      itrTo = m_states.insert(std::make_pair(to,StatePtrList())).first;

   auto& fromList = itrFrom->second;
   auto& toList = itrTo->second;
   //---------------------------------------------------------------------------
   // splice_after: Transfers elements from fromList into toList inserting  them
   // after the element pointed by position.   This  effectively  inserts  those
   // elements into toList and removes them from fromList, altering the sizes of
   // both containers.  The operation  does  not  involve  the  construction  or
   // destruction of any element. 
   //---------------------------------------------------------------------------
   toList.splice_after(toList.cbefore_begin(), fromList);
   fromList.clear();
   m_states.erase(from);
   constRunTime = purgeStateUsage(to) && constRunTime;
   return constRunTime;
}

template<typename NODE_CLASS, typename STATE_ENUM>
   bool 
CppStateManager<NODE_CLASS, STATE_ENUM>::migrateAllStatesTo(STATE_ENUM to)
{
   bool constRunTime = true;
   // to avoid iterator invalidation, first store all keys.
   std::vector<STATE_ENUM> fromStates;
   for(auto& s : m_states)
   {
      if(s.first != to)
         fromStates.push_back(s.first);
   }
   for(auto from : fromStates)
      constRunTime = migrateGivenStateTo(from,to) && constRunTime ;
   return constRunTime;
}

template<typename NODE_CLASS, typename STATE_ENUM>
   bool 
CppStateManager<NODE_CLASS, STATE_ENUM>::swapStates(STATE_ENUM one, STATE_ENUM two)
{
   bool constRunTime = true;
   if(one==two)
      return constRunTime;

   auto itrOne = m_states.find(one);
   if(itrOne == m_states.end())
      itrOne = m_states.insert(std::make_pair(one,StatePtrList())).first;

   auto itrTwo = m_states.find(two);
   if(itrTwo == m_states.end())
      itrTwo = m_states.insert(std::make_pair(two,StatePtrList())).first;

   auto& oneList = itrOne->second;
   auto& twoList = itrTwo->second;
   std::swap(oneList,twoList);
   constRunTime = purgeStateUsage(one) && constRunTime ;
   constRunTime = purgeStateUsage(two) && constRunTime;
   return constRunTime;
}

template<typename NODE_CLASS, typename STATE_ENUM>
   bool 
CppStateManager<NODE_CLASS, STATE_ENUM>::purgeStateUsage(STATE_ENUM s)
{
   bool constRunTime = true;
   auto itr = m_states.find(s);
   if(itr != m_states.end())
   {
      StatePtrList& sList = itr->second;
      size_t fragments = 0;
      size_t counter = 0;
      auto itrBefore = sList.before_begin();
      auto itrNext = sList.begin();
      while(itrNext != sList.end())
      {
         auto useCount = (*itrNext)->nodes.size();
         if(useCount == 0)
         {
            delete (*itrNext);
            itrNext = sList.erase_after(itrBefore);
            continue;
         }
         ((*itrNext)->state) = s; // make sure that map with index s has all values as s
         counter += useCount;
         ++fragments;
         ++itrBefore;
         ++itrNext;
      }
      //------------------------------------------------------------------------
      // Defragmentation logic: If on avg for every 3%  or  less  nodes  we  are
      // burning a fragment, then we have too  many  fragments  so  let's  do  a
      // defragmentation.  The threshold 3% is a heuristic and was decide  using
      // experiments in a node traversal problem.
      //------------------------------------------------------------------------
      if(fragments>0 && ((float)counter/fragments)<=3)
      {
         defragmentStateUsage(s);
         constRunTime = false;
      }
   }
   return constRunTime;
}

template<typename NODE_CLASS, typename STATE_ENUM>
   void 
CppStateManager<NODE_CLASS, STATE_ENUM>::defragmentStateUsage(STATE_ENUM s)
{
   // Logic: iterate over all fragments of type s and merge them to 1st fragment
   auto itr = m_states.find(s);
   if(itr != m_states.end())
   {
      StatePtrList& sList = itr->second;
      auto itrFirst = sList.begin();
      if(itrFirst == sList.end())
         return; // Empty forward_list
      auto itrSecond = sList.begin(); ++itrSecond;
      while(itrSecond != sList.end())
      {
         for(auto n : (*itrSecond)->nodes)
         {
            (*itrFirst)->nodes.insert(n);
            n->mp_state = *itrFirst;
         }
         (*itrSecond)->nodes.clear(); 
         delete (*itrSecond);
         itrSecond = sList.erase_after(itrFirst);
      }
   }
}

template<typename NODE_CLASS, typename STATE_ENUM>
   void 
CppStateManager<NODE_CLASS, STATE_ENUM>::printStateUsage(unsigned int lineNo) 
{
   std::cout << "State usage";
   if(lineNo>0)
      std::cout << " at line "<< lineNo ;
   std::cout << ":" << std::endl;
   unsigned int totalCounter = 0;
   for(auto& mapElem : m_states)
   {
      size_t counter = 0;
      STATE_ENUM s = mapElem.first;
      StatePtrList& sList = mapElem.second;
      unsigned int fragments = 0;
      auto itrBefore = sList.before_begin();
      auto itrNext = sList.begin();
      while(itrNext != sList.end())
      {
         auto useCount = (*itrNext)->nodes.size();
         counter += useCount;
         if((*(*itrNext)) != s) // make sure that map with index s has all values as s
            std::cerr << ((*itrNext)->state) << " state found instead of state " << s << std::endl;
         ++fragments;
         ++itrBefore;
         ++itrNext;
      }
      if(counter>0)
      {
         std::cout << "\t" << mapElem.first << " -> " << counter 
            << " (in " << fragments << " fragments)" << std::endl;
         totalCounter += counter;
      }
   }
   std::cout << "\t" << "Total" << " -> " << totalCounter << std::endl << std::endl;
}

template<typename NODE_CLASS, typename STATE_ENUM>
unsigned int 
CppStateManager<NODE_CLASS, STATE_ENUM>::getStateUsage(STATE_ENUM s) const
{
   unsigned int counter = 0;
   auto itr = m_states.find(s);
   if(itr == m_states.end())
      return 0;

   const StatePtrList& sList = itr->second;
   auto itrNext = sList.begin();
   while(itrNext != sList.end())
   {
      counter += (*itrNext)->nodes.size();
      if((*(*itrNext)) != s) // make sure that map with index s has all values as s
         std::cerr << ((*itrNext)->state) << " state found instead of state " << s << std::endl;
      ++itrNext;
   }

   return counter;
}

template<typename NODE_CLASS, typename STATE_ENUM>
void 
CppStateManager<NODE_CLASS, STATE_ENUM>::getStateNodes(STATE_ENUM s, std::vector<NODE_CLASS*>& nodes) const
{
   auto itr = m_states.find(s);
   if(itr == m_states.end())
      return ;

   const StatePtrList& sList = itr->second;
   auto itrList = sList.begin();
   while(itrList != sList.end())
   {
      if((*(*itrList)) != s) // make sure that map with index s has all values as s
         std::cerr << ((*itrList)->state) << " state found instead of state " << s << std::endl;
      std::copy((*itrList)->nodes.begin(),(*itrList)->nodes.end(),std::back_inserter(nodes));
      ++itrList;
   }
}

#undef CppStateManagerDllExportMacro
#endif // CppStateManager_h_
