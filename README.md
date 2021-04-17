# states

Use this library to create state machines with very little runtime overhead.

##To create a state machine:
1. Determine the data to be passed around the state machine.  This might be a new struct.  Or it might be the owning class of the state machine.  
Either way, it will be passed around as a reference.

ex:
'''
struct Data
{
    int num_tries_;
    double value_;
};
'''

2. Create some states:
    - Create some names:
ex:
'''
static const char sBegin[] = "Begin";
static const char sProcessor[] = "Processor";
static const char sChecker[] = "Checker";
static const char sEnd[] = "End";
'''

    - Create some states:
ex:
'''
typedef states::State<sBegin> Begin;
typedef states::State<sProcessor> Processor;
typedef states::State<sChecker> Checker;
typedef states::State<sEnd> End;
'''

    - If operations are required to occur when arriving at a state or using dispatch (see below), create some operations:
ex:
'''
struct Reset
{
void operator()(Data& d) { d.num_tries_ = 0; d.value_ = 0; }
};
'''

    - Attach the operations to the states when defining them as typedefs:
ex:
instead of:
'''
typedef states::State<sBegin> Begin;
'''
do:
'''
typedef states::State<sBegin, Reset> Begin;
'''

2.Create some events:
    - Create some names:
ex:
'''
static const char eStart[] = "Start";
static const char eCheck[] = "Check";
static const char eCheckFailed[] = "Check Failed";
static const char eCheckPassed[] = "Check Passed";
'''

    - Create some events:
ex:
'''
typedef states::Event<eStart> Start;
typedef states::Event<eCheck> Check;
typedef states::Event<eCheckFailed> CheckFailed;
typedef states::Event<eCheckWorked> CheckWorked;
'''

3. Create some links.  Links are the transitions from state to state that occur due to an event.
    - Create some links:
ex:
'''
typedef states::Link<Begin, Start, Processor> Link1;
typedef states::Link<Processor, Check, Checker> Link2;
typedef states::Link<Checker, CheckFailed, Processor> Link3;
typedef states::Link<Checker, CheckPassed, End> Link4;
'''

If operations are required to occur when traversing a link, create some operations:
ex:
'''
struct Reset
{
void operator()(Data& d) { d.num_tries_ = 0; d.value_ = 0; }
};
'''

    - Attach the operations to the states when defining them as typedefs:
ex:
instead of:
'''
typedef states::Link<Checker, CheckFailed, Processor> Link3;
'''
do:
'''
typedef states::Link<Checker, CheckFailed, Processor, Reset> Link3;
'''

4. Create a machine using the set of links
    - create the machine:
ex:
'''
typedef states::Machine<Link1, Link2, Link3, Link4> MachineType;
'''

5. Create a process to use the machine:
    - Create the process using:
ex:
'''
typedef states::Process<Start, End, MachineType, Data> ProcessType;
'''

b. Traverse through the process by creating an instance and using its functions:
ex:
Data d;
ProcessType p(d);
p.start();
p.next<Start>();
assert(p.at<Processor>());
p.next<Check>();
p.next<CheckPassed>();
assert(p.done());

FAQ:
1. Is there a way to verify that the machine is traversable?
    No. Not yet.  An issue has been raised to do this.  It is planned to do some template ninja magic to make the Process have a 
    static_assert that will keep it from having TEnd unreachable from TBegin using the set of TLinks in TMachine.  Compile time
    checks have been added for making sure TBegin is in the set of TFrom in the TLinks and that TEnd is in the set of TTo in the
    TLinks.  
    
2. Why doesn't process automatically start the process?  Why is start required to be called?
    Process begins in an invalid state.  Start must be called before any next functions.  This is because there is an operation
    associated with each state and if start was called automaticly, that operation would be called.  It seemed like this would
    not be optimal if the process was both a data member of a class and held a reference to its owning class.  This could cause
    problems if the process was owned (and created during construction of) the object it also holds a reference to.  A call onto 
    its functions might be made before it was fully constructed.
    
3. Why doesn't this compile?
    Compile time checks have been added for Process (see #1 above) and for checking that the set of Links do not have 
    multiple links with the same TBegin and TEvent.  Also, the StateNum and EventNum will not compile if using States or Events
    that are not in the list for the set and is functions.
    
4. How can the state machine be dumped?
    The name parameter for the state and event will be used in a future revision which will include this feature.  An issue has
    been raised.
    
5. Using a paramterized next is inconvenient because the next state indicator can't be saved in a variable.  What can be done?
    Use Process<>::TEventNum to be the event to be pushed to next:
    
    '''
    Data d;
    ProcessType p(d);
    p.start();
    p.next<Start>(); //line 4
    '''
    replace line 4 with:
    '''
    ProcessType::TEventNum event;
    event.set<Start>();
    p.next(event);
    '''
    Variable event can be stored.
    
6. How can dispatch be performed?  Based on the current state, the state operation should be performed.
    Use the Process invoke function.  If the process is at a valid state, the state's operation will be performed without 
    changing state.  Without this function, a link with the same begin and end and a dispatch event would be necessary
    in the links.
    
7. Can the process be re-used?
    Use the reset function to set the process back to the newly constructed "no-state."  Or call start to set the process
    to the Begin state (and call the operation as necessary.)  Without the first, it would be necessary to destruct and
    construct the process.  Without the second it would be necessary to either destruct and construct and call start, or
    to have links and a dedicated event from all states back to start.
    
  8. What happens if a valid event is used but it is not valid for the current state?  
      The next function will not move the state, no operations will be called.  The next function will return false.
