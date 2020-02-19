
#include <memory>
#include <unordered_map>
#include <forward_list>
#include "./../CppEnumCreator/src/CppEnumCreatorIncludes.h"

#ifndef CppStateManager
#define CppStateManager CppStateManager
#endif

#ifndef CppStateList
#error "Please define CppStateList macro before including CppStateManagerDeclaration.h file."
#endif

class CppStateManager
{
   private:
      //------------------------------------------------------------------------
      // Using singleton design pattern for CppStateManagerClass.
      //------------------------------------------------------------------------
      CppStateManager(  );
      ~CppStateManager(  );
      CppStateManager (const CppStateManager& obj ) = delete;
      CppStateManager& operator=( const CppStateManager& obj ) = delete;

   public:
      static CppStateManager& getGlobal();

      // Define Enums with all its bells and whistles.
#define CppDllExportMacro /*Give name of the Dll Export macro if needed*/
#define CppEnumName States
#define CppEnumParentClass CppStateManager
#define CppEnumList CppStateList
#include "./../CppEnumCreator/src/CppEnumCreatorDeclaration.h"

      typedef std::shared_ptr<const States> ConstStatePtr;
      ConstStatePtr getStatePtr(States s);
      ConstStatePtr getStatePtr(std::string sStr);
      void purgeStateUsage();
      void printStateUsage(unsigned int lineNo=0);
      void migrateState(States from, States to);
      void migrateState(std::string fromStr, std::string toStr);
      void swapStates(States one, States two);
      void swapStates(std::string oneStr, std::string twoStr);
   private:
      typedef std::shared_ptr<States> StatePtr;
      typedef std::forward_list<StatePtr> StatePtrList;
      std::unordered_map<States, StatePtrList> m_states;
};

#undef CppStateManager
#undef CppStateList
