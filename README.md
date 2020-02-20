# CppStateManager
Set of macros to elegently and automatically define CppStateManager class that can act as a central manager to hold states (e.g. WHITE/BLACK/GRAY, UP_TO_DATE/STALE, VISITED/NOT_VISITED etc.) for any group of objects (nodes in a graph, cells in a spreadsheet, gates in a circuit, etc.)

### Motivation
Most algorithms that work with a large group of objects, often has a preamble stage of iterating over these objects and changing something in their state. For example, iterating over all nodes in a graph and changing their color to WHITE (not visited) before a traversal. Or Invalidating (marking STALE) cells in a spreadsheet type application before a full-calculation. Or resetting arrival-time/slack at every gate before static timing analysis of a circuit. Sometimes, we also requires stats like what is the count of gates that are not meeting their slack.

One solution to do this, is to store the state inside every object as some sort of boolean or enum. And then iterating over all objects to reset them or count the stats. But this often makes an O(N) iteration where N is the number of objects.

### Description
This project provides an alternative way of implementing the state property of every object by creating a [singleton](https://en.wikipedia.org/wiki/Singleton_pattern) state-manager class and storing [fly-weight](https://en.wikipedia.org/wiki/Flyweight_pattern) objects in the manager and sharing their shared_ptrs with individual objects. This enables an ~O(1) state migration or state swapping for all the objects by doing that in one-shot. It also provides APIs to printUsage or getUsage for all or one of the states using shared_ptr's use_counts.
Note: the same approach can be easily extended to not only return counts of objects in a particular state but also list of all objects that are in that state. Give it a try if your application requires that.

### Dependencies
The code uses [CppEnumCreator](https://github.com/gandhidarshak/CppEnumCreator) project to create the state enums.
The code is tested on Linux/Windows-Cygwin gcc-6.2.0. However, It should work on any recent C++ compilers that supports C++ 11 or above. I have purposefully avoided dependencies on special purpose  libraries like Boost to make it self sufficient and easy to use. 

### Installation 
Installation is quite simple, git clone https://github.com/gandhidarshak/CppStateManager <PathOfDir> --recursive will download this and all other dependent submodules.

### Usage
The state manager class creation works in 2 steps - declaration/definition, like most other code that you will write between header and c++ file.
1.  header file - declaration:
Inside your header file, #define below macros. And #include "CppStateManagerDeclaration.h" after that. 
 
    1.  CppStateList <List of possible states>
    2.  CppStateManager <Name of manager class> (optional. defaulted to CppStateManager if not given)
    3.  CppStateManagerDllExportMacro <Name of the dll export/import macro if working across dlls in windows.> (optional)
2.  C++ file - definition:
Inside your c++ file, define CppStateManager class' name if you have over-ridden the default above and #include  "CppStateManagerDefinition.h" after that.

After the above two steps, upon compilation, you will automatically have a class with below typedefs/APIs declared (in .h) and defined (in .c++). Some of these APIs (#13 onwards) are derived for the ennum class as mentioned in [CppEnumCreator](https://github.com/gandhidarshak/CppEnumCreator)
1. getGlobal() to get the singleton by reference
2. typedef std::shared_ptr<const States> ConstStatePtr;
3. getStatePtr(States s) - get shared_ptr to assign to local ptr variable of the object
3. getStatePtr(std::string sStr) - as above with string inputs.
5. purgeStateUsage() - purge the fly-weight objects which are no longer referenced by anything
6. printStateUsage(unsigned int lineNo=0) - print usage stats for all stats.
7. migrateState(States from, States to) - migrate all objects of state "from" to state "to"
8. migrateState(std::string fromStr, std::string toStr) - as above with string inputs
9. swapStates(States one, States two) - swap objects of state one and two with each other
10. swapStates(std::string oneStr, std::string twoStr) - as above with string inputs
11. unsigned int getStateUsage(States s) - get number of objects with give state
12. unsigned int getStateUsage(std::string sStr) - as above with string inputs
13. string_to_enum
14. enum_to_string
15. ushort_to_enum* 
16. enum_to_ushort*
17. prefix decrement (--x) operator
18. postfix decrement (x--) operator
19. prefix increment (\++x) operator
20. postfix increment (x++) operator
21. ostream << operator 
22. num_enum_items

Please see [test.h](https://github.com/gandhidarshak/CppStateManager/blob/master/test/test.h) and [test.cxx](https://github.com/gandhidarshak/CppStateManager/blob/master/test/test.cxx) files for a more detailed example of the usage. 

### Sharing is caring!

Please feel free to IM me with your suggestions and feedback about the idea!

### License

CppStateManager library uses the MIT license. See [LICENSE](https://github.com/gandhidarshak/CppStateManager/blob/master/LICENSE.md) for more details.
