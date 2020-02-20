# CppStateManager
Set of macros to elegently and automatically define CppStateManager class that can act as a central manager to hold states (e.g. WHITE/BLACK/GRAY, UP_TO_DATE/STALE, etc.) for all the nodes of any graph representation or all the cells of any spreadsheet representation.

<!---
### Motivation
Ever since I learned C++ for the first time in ~2007, I have found my self using user defined enum data-types to define different type of explicitely named constants. More often than not, for these enums, I have found one or the other need to have a enum_to_string and/or string_to_enum conversion which often require us to write the same words multiple times (as string, as enums, etc.) which is error prone and difficult to maintain over time. Albeit, there are some preprocessor macro driven approaches ([e.g.](https://stackoverflow.com/questions/5530248/creating-a-string-list-and-an-enum-list-from-a-c-macro)) which helps in defining the list only once and providing both conversion, but they are far from being easy to use and elegant.

With C++11, enum classes (tightly scoped enums) were introduced which I immidiately got attracted to due to their [advantages](https://www.geeksforgeeks.org/enum-classes-in-c-and-their-advantage-over-enum-datatype) over the plain old enum data types. However, with the virtue of having type-safety (no default conversion between enum and underlying int representations), enum classes often poses innate need of APIs like enum_to_number and number_to_enum and increment/decrement operators like ++/-- .

### Description
This project developes an easy/intuitive method to define/declare enum class types inside any class or namespace. All that the users have to do is provide name of enum, its possible values (list) and name of the class (if applicable) as predefined macros in .h (declaration) and .cxx (definition) files of their code. With proper inclusion of files developed in this project, macros defined inside the file creates the enum class and automatically defines/declares static APIs that performs everything mentioned above.

### Dependencies
The code is tested on Linux/Windows-Cygwin gcc-6.2.0. However, It should work on any recent C++ compilers that supports C++ 11 or above. I have purposefully avoided dependencies on special purpose  libraries like Boost to make it self sufficient and easy to use. 

### Installation 
Installation is quite simple, just download the .h files from [src dir](https://github.com/gandhidarshak/CppStateManager/tree/master/src/) from git hub and keep it in a location which is accessible from your project.  

### Usage
The enum creation works in 2 steps - declaration/definition, like most other code that you will write between header and c++ file.
1.  header file - declaration:
Inside your header file, inside the class/namespace declaration {...}, where you will normally define enums, #define below macros. And #include  "CppStateManagerDeclaration.h" after that. The files uses standard C++ includes, but if needed, you can also #include "CppStateManagerIncludes.h" at top of your header file.
    1.  CppEnumParentClass <Name of the class/namespace - e.g. classA, namespaceB, namespaceC::ClassA>
    2.  CppEnumName <Name of the enum class - e.g. Digits, Colors, Months>
    3.  CppEnumList <Comma separated list of enum-items. Use \ for multine line spliting.>
    4.  CppDllExportMacro <Name of the dll export/import macro if working across dlls in windows.> (optional)
2.  C++ file - definition:
Inside your c++ file, where you will normally define class APIs or enum APIs, copy below two macros from the header file above. And #include  "CppStateManagerDefinition.h" after that.
    1.  CppEnumParentClass <Name of the class/namespace - e.g. classA, namespaceB, namespaceC::ClassA>
    2.  CppEnumName <Name of the enum class - e.g. Digits, Colors, Months>

After the above two steps, upone compilation, you will automatically have below static APIs declared (in .h) and defined (in .c++) in your class or namespace.
1.  string_to_enum
2.  enum_to_string
3.  ushort_to_enum* 
4.  enum_to_ushort*
5.  prefix decrement (--x) operator
6.  postfix decrement (x--) operator
7.  prefix increment (\++x) operator
8.  postfix increment (x++) operator
9.  ostream << operator 
10. num_enum_items

*unsigned short is used as underlying number representation of enum



Please see [test.h](https://github.com/gandhidarshak/CppStateManager/blob/master/test/test.h) and [test.cxx](https://github.com/gandhidarshak/CppStateManager/blob/master/test/test.cxx) files for a more detailed example of the usage. 

-->
### Sharing is caring!

Please feel free to IM me with your suggestions and feedback about the idea!

### License

CppStateManager library uses the MIT license. See [LICENSE](https://github.com/gandhidarshak/CppStateManager/blob/master/LICENSE.md) for more details.
