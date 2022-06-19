# Events
 
This library is an implementation of a Event Signal and Slot concept, which allows connecting Signals and Slots between function and class methods. It was implemented like the QT Signal Slot concept but without any moc compiler.
 
This library was forked from
- https://github.com/tomstewart89/Callback
 
and updated with further features.
 
The implementation is template based and allows you to create signals and slots with
- No Parameter
- Multiple Parameter
 
For the easy usage of the Signals and Slots, there are few generic marcos which should be used.
The following examples show how the library can be used.
 
## Include Events
 
```c++
#include "Events.h"
 
void setup() {
 
}
```
 
## Using Signals
 
Creation of a Signal can be done in the following way.
 
```c++
 
class Base {
 
    public:
        EVENT_SIGNAL(click);                  //Signal click without any parameter
        EVENT_SIGNAL(triggered,bool);          //Signal triggered with one parameter
        EVENT_SIGNAL(values,int,float,int);    //Signal values with three parameter
};
 
```
Each of these signal will be extended to a parameter with the following form
- m_click_signal;
- m_triggered_signal
- m_value_signal;
 
These parameters are the signals which can be used for the EVENT_CONNETC_SIGNAL2x macros.
 
## Register Event Lib For Slots
 
The marco name is "EVENT_REG_FOR_SLOTS()".
This macro creates some code, which makes the slot usage a lot easier. The macro needs a parameter and this parameter has to be the class name where you like to create the slots.
 
Example:
```c++
 
class Base {
 
    EVENT_REG_FOR_SLOTS(Base)
    ...
};
 
```
 
## Using Slots
 
The creation of Slots works the same way like the Signals. Only one additional marco needs to be added to allow the creation of the slot.
 
Example: Definition of Slots
```c++
 
class Base {
 
    EVENT_REG_FOR_SLOTS(Base)               //Required for Slots!!!!
   
 
    public:
        EVENT_SLOT(onClick);                  //Signal click without any parameter
        EVENT_SLOT(onTriggered,bool);          //Signal triggered with one parameter
        EVENT_SLOT(onValues,int,float,int);    //Signal values with three parameter
};
 
```
Each of these slots will also be extended to a parameter with the following form
    - m_onClick_slot;
    - m_onTriggerd_slot
    - m_onValues_slot;
 
The next step is to implement the Slot methods, and this is quite simple. Each Slot needs a class method with the following structure:
 
```c++
    void <USED_SLOT_NAME>Slot( *<PARAMETER>*){};
```
 
These methods will be called each time a signal will be emitted.
 
Example: Implementation of Slots
```c++
class Base {
 
    EVENT_REG_FOR_SLOTS(Base)               //Required for Slots!!!!
   
    private:
        void onClickSlot(void) {}
        void onTriggeredSlot(bool status) {}
        void onValues(int i, flot j, int k) {}
    public:
        EVENT_SLOT(onClick);                  //Signal click without any parameter
        EVENT_SLOT(onTriggerd,bool);          //Signal triggered with one parameter
        EVENT_SLOT(onValues,int,float,int);    //Signal values with three parameter
};
 
```
 
## Connect Signal to Slots
 
It is possible to connect Signals to Slot, but also Signals to Signals. The Signal to Signal connection is a good solution in case of Subclasses. There are one macro which can be used to connect Signals and Signals/Slots.
 
```c++
    EVENT_CONNECT();
    
```
 
Example: Connect Signal to Slots
 
```c++
#include "Events.h"
 
Base sender;
Base receiver;
 
void setup(void)
{
    EVENT_CONNECT(sender.m_click_signal, receiver.m_onClick_slot);
}
```
 
 
## Emit a signal
 
To send data between signal and slot, there are two possibilities. Each signal includes an "emit" method. This method has to be used to emit signals and depends on the number of defined arguments of the Signal. The second possibility is to use a macro.
 
Example: Emit Signals
 
```c++
 
 
...
 
Base sender;
Base receiver;
 
...
 
void loop()
{
    sender.m_click_signal.emit();
    EVENT_EMIT(sender.m_click_signal);  
 
    sender.m_onTriggerd_slot.emit(true);
    EVENT_EMIT(sender.m_onTriggerd_slot, true);
 
    sender.m_onValues_slot.emit(42,3.14,42);
    EVENT_EMIT(sender.m_onValues_slot, 42,3.14,42);
 
    delay(2)
}
 
```
