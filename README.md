# CppStateManager
Set of macros to elegently and automatically define CppStateManager class that can act as a central manager to hold states (e.g. WHITE/BLACK/GRAY, UP_TO_DATE/STALE, VISITED/NOT_VISITED etc.) for any group of objects (nodes in a graph, cells in a spreadsheet, gates in a circuit, etc.)

### Motivation
Most algorithms that work with a large group of objects, often has a preamble stage of iterating over these objects and changing something in their state. For example, iterating over all nodes in a graph and changing their color to WHITE (not visited) before a traversal. Or Invalidating (marking STALE) cells in a spreadsheet type application before a full-calculation. Or resetting arrival-time/slack at every gate before static timing analysis of a circuit. Sometimes, we also requires stats like what is the count of gates that are not meeting their slack. More often than not, we definitely come in situation where we want to iterate over all Nodes of a perticular state. Like iterate over all dirty cells.

One solution to do this, is to store the state inside every object as some sort of boolean or enum. And then iterating over all objects to reset them or count the stats or to visit objects of one perticular state. But this often makes an O(N) iteration where N is the number of objects.

### Description
This project provides an alternative way of implementing the state property of every object by creating a [singleton](https://en.wikipedia.org/wiki/Singleton_pattern) state-manager class and storing [fly-weight](https://en.wikipedia.org/wiki/Flyweight_pattern) objects in the manager and sharing their pointers with individual objects. This enables an ~O(1) state migration or state swapping for all the objects by doing that in one-shot. It provides ~O(k) API to populate vector if all k objects in one particular state. It also provides O(1) APIs to printUsage or getUsage for all or one of the states.

At a high-level, this implementation uses more memory to provide amortized constant run-time for various state analysis requirements. It also implements a auto-triggering defragmentation mechanism which keeps the run-time constant over extended usage of the class.

### Dependencies
The code is tested on Linux/Windows-Cygwin gcc-6.2.0. However, It should work on any recent C++ compilers that supports C++ 11 or above. I have purposefully avoided dependencies on special purpose  libraries like Boost to make it self sufficient and easy to use. 
Note: The test code uses [CppEnumCreator](https://github.com/gandhidarshak/CppEnumCreator) project to create the state enums. But if you want to avoid using that enum creator, you can use any other enum as well.
Note: This class is not strictly thread-safe due to the nature of the problem that I was trying to solve while creating this. If you want thread-safety, please put mutex locks on usage of m_states variable in class' APIs.

### Installation 
Installation is quite simple, git clone https://github.com/gandhidarshak/CppStateManager <PathOfDir> --recursive will download this and all other dependent submodules.

### Usage
CppStateManager/CppStateNodesBundle are template classes which takes two
arguments - NodeClass and EnumOfStates.  Please see
[node.h](https://github.com/gandhidarshak/CppStateManager/blob/master/test/node.h)
and
[node.cxx](https://github.com/gandhidarshak/CppStateManager/blob/master/test/node.cxx)
files for a detailed explanation on how you can derive your own state manager from the base template and then use it in your code.

After the above step, upon compilation, you will automatically have a class with below APIs.
1. getGlobal() to get the singleton by reference
2. getState(Node*) - get state for given node. This can be used in node class to simplify.
3. setState(Node*, StateEnum) - set state for given node. This can be used in node class to simplify.
4. clearState(Node*) - to clear the state of a node.Typically called while deleting a node.
5. purgeStateUsage() - purge the fly-weight objects which are no longer referenced by anything
6. printStateUsage(unsigned int lineNo=0) - print usage stats for all stats.
7. migrateGivenStateTo(StateEnum from, StateEnum to) - migrate all objects of state "from" to state "to"
8. migrateAllStatesTo(StateEnum to) - migrate all objects of any state to state "to"
9. swapStates(StateEnum one, StateEnum two) - swap objects of state one and two with each other
10. getStateUsage(States s) - get number of objects with give state
11. getStateNodes(States s, vector<node*>&) - populates vector with all nodes in given state.
12. defragmentStateUsage(States s) - Over time, a state's node may get dispersed in different fragments, which can be defragment using this API. It get's auto-triggered inside migrate APIs when needed.

Please see [test.cxx](https://github.com/gandhidarshak/CppStateManager/blob/master/test/test.cxx) and it's output [test.output](https://github.com/gandhidarshak/CppStateManager/blob/master/test/test.output) files for better understanding the API's usage.

### Sharing is caring!

Please feel free to IM me with your suggestions and feedback about the idea!

### License

CppStateManager library uses the MIT license. See [LICENSE](https://github.com/gandhidarshak/CppStateManager/blob/master/LICENSE.md) for more details.
