
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

#ifndef CppStringify
#define CppStringify_(...) #__VA_ARGS__
#define CppStringify(x) CppStringify_(x)
#endif

#ifndef CppStateManagerDllExportMacro 
#define CppStateManagerDllExportMacro /*No Dll Export?*/
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
      CppStateManagerDllExportMacro static CppStateManager& getGlobal();

      // Define Enums with all its bells and whistles.
#define CppEnumName States
#define CppEnumParentClass CppStateManager
#define CppEnumList CppStateList
#define CppEnumCreatorDllExportMacro CppStateManagerDllExportMacro
#include "./../CppEnumCreator/src/CppEnumCreatorDeclaration.h"

      typedef std::shared_ptr<const States> ConstStatePtr;
      CppStateManagerDllExportMacro ConstStatePtr getStatePtr(States s);
      CppStateManagerDllExportMacro ConstStatePtr getStatePtr(std::string sStr);
      CppStateManagerDllExportMacro void purgeStateUsage();
      CppStateManagerDllExportMacro void printStateUsage(unsigned int lineNo=0);
      CppStateManagerDllExportMacro void migrateState(States from, States to);
      CppStateManagerDllExportMacro void migrateState(std::string fromStr, std::string toStr);
      CppStateManagerDllExportMacro void swapStates(States one, States two);
      CppStateManagerDllExportMacro void swapStates(std::string oneStr, std::string twoStr);
      CppStateManagerDllExportMacro unsigned int getStateUsage(States s);
      CppStateManagerDllExportMacro unsigned int getStateUsage(std::string sStr);
   private:
      typedef std::shared_ptr<States> StatePtr;
      typedef std::forward_list<StatePtr> StatePtrList;
      std::unordered_map<States, StatePtrList> m_states;
};

#undef CppStateManagerDllExportMacro
#undef CppStateManager
#undef CppStateList
