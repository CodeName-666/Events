
# Events

This library is implementation of a Event Signal and Slot concepet, wich allows to connect Signals and Slots between function and class methods. It was implemented like the QT Signal Slot concept but without any moc compiler.

This library was forked from 
- https://github.com/tomstewart89/Callback 

and updated with further features.

The implementation is template based and allows you to craete signals and slots whith
- No Parameter
- Multiple Parameter

For the easy usage of the Signals and Slots, there are few generic marcos which should be used.
The following examples shows how the library can be used.

## Include Events 

```c++
#include "Events.h"

void setup() {

}
```

## Using Signals

Creation of a Signale is can be done in the following way. 

```c++

class Base {

    public: 
        EVENT_SIGNAL(click);                  //Signal click without any paramter
        EVENT_SIGNAL(triggerd,bool);          //Signal triggerd with one parameter
        EVENT_SIGNAL(values,int,flot,int);    //Signal values with three parameter
}; 

```
Each of these signal will be extended to a parameter with the following form
    - m_click_signal;
    - m_triggered_signal
    - m_value_signal;

These parameter are the signals which can be used for the EVENT_CONNETC_SIGNAL2x macros.

## Register Event Lib For Slots

 The marco name is "EVENT_REG_FOR_SLOTS()".
This marco creates some code, wich makes the slot usage a lot of easier. The macro need on paramter and this paramete has to be the class name where you like to create the slots.

Example:
```c++

class Base {

    EVENT_REG_FOR_SLOTS(Base)
    ...
};

```

## Using Slots

The creation of Slots works the same way like the Signals. Only one additional marco needs to be added to allow the createon of the slot.

Example: Definition of Slots
```c++

class Base {

    EVENT_REG_FOR_SLOTS(Base)               //Required for Slots!!!!
    

    public:
        EVENT_SLOT(onClick);                  //Signal click without any paramter
        EVENT_SLOT(onTriggerd,bool);          //Signal triggerd with one parameter
        EVENT_SLOT(onValues,int,flot,int);    //Signal values with three parameter
};

```
Each of these slots will also be extended to a parameter with the following form
    - m_onClick_slot;
    - m_onTriggerd_slot
    - m_onValues_slot;

The next step is to implement the Slot methods, and this is quite simple. Each Slot needs a class method with the following structre:

```c++
    void <USED_SLOT_NAME>SLOT(<PARAMETER>){};
```

These methods will be called each time, a signal will be emitted.

Example: Implementation of Slots
```c++
class Base {

    EVENT_REG_FOR_SLOTS(Base)               //Required for Slots!!!!
    
    private:
        void onClickSlot(void) {}
        void onTriggerdSlot(bool status) {}
        void onValues(int i, flot j, int k) {}
    public:
        EVENT_SLOT(onClick);                  //Signal click without any paramter
        EVENT_SLOT(onTriggerd,bool);          //Signal triggerd with one parameter
        EVENT_SLOT(onValues,int,flot,int);    //Signal values with three parameter
};

```

## Connect Signal to Slots

It is possible to connect Signals to Slot, but also Signals to Signals. The Signal to Signal connection is a good solution in case of Subclasses. There are also two macros which can be used to connect Siganls and Signals/Slots.

```c++
    EVENT_CONNECT_SIGNAL2SIGNAL();
    EVENT_CONNECT_SIGNAL2SLOT();
```

Example: Connect Signal to Slots

```c++
#include "Events.h"

Base sender;
Base receiver;

void setup(void)
{
    EVENT_CONNECT_SIGNAL2SLOT(sender.m_click_signal, receiver.m_onClick_slot);
}
```


## Emit a signal

To send data between signal and slot, there are two possibilites. Each signal includes a "emit" method. This method has to be used to emit signals and depands on the number of defined arguments of the Signal. The second possibility is to use a macro.

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