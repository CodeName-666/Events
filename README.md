# Events

This library is an implementation of an Event Signal and Slot concept, which allows connecting Signals and Slots between functions and class methods. It was implemented like the Qt Signal-Slot concept but without any MOC compiler.

This library was forked from:

- https://github.com/tomstewart89/Callback

and updated with further features.

The implementation is template-based and allows you to create signals and slots with:

- No parameters
- Multiple parameters

For easy usage of the Signals and Slots, there are a few generic macros which should be used. The following examples show how the library can be used.

## Include Events

```c++
#include "Events.h"

void setup() {

}
```

## Using Signals

Creation of a Signal can be done in the following way:

```c++
class Base {

    public:
        EVENT_SIGNAL(click);                      // Signal 'click' without any parameters
        EVENT_SIGNAL(triggered, bool);            // Signal 'triggered' with one parameter
        EVENT_SIGNAL(values, int, float, int);    // Signal 'values' with three parameters
};
```

Each of these signals will be expanded to a member variable with the following form:

- `m_click_signal;`
- `m_triggered_signal;`
- `m_values_signal;`

These member variables are the signals which can be used with the `EVENT_CONNECT` macro.

## Register Event Library for Slots

The macro name is `EVENT_REGISTER_EVENTS()`.

This macro generates some code which makes the slot usage a lot easier. The macro needs a parameter, and this parameter has to be the class name where you would like to create the slots.

Example:

```c++
class Base {

    EVENT_REGISTER_EVENTS(Base)
    // ...
};
```

## Using Slots

The creation of Slots works similarly to the creation of Signals. Only one additional macro needs to be added to allow the creation of the slot.

Example: Definition of Slots

```c++
class Base {

    EVENT_REGISTER_EVENTS(Base)                   // Required for Slots!!!!

    public:
        EVENT_SLOT(onClick);                      // Slot 'onClick' without any parameters
        EVENT_SLOT(onTriggered, bool);            // Slot 'onTriggered' with one parameter
        EVENT_SLOT(onValues, int, float, int);    // Slot 'onValues' with three parameters
};
```

Each of these slots will also be expanded to a member variable with the following form:

- `m_onClick_slot;`
- `m_onTriggered_slot;`
- `m_onValues_slot;`

The next step is to implement the Slot methods, and this is quite simple. Each Slot needs a class method with the following structure:

```c++
void <USED_SLOT_NAME>Slot(<PARAMETERS>) {
    // Slot implementation
}
```

These methods will be called each time a signal is emitted.

Example: Implementation of Slots

```c++
class Base {

    EVENT_REGISTER_EVENTS(Base)                   // Required for Slots!!!!

    private:
        void onClickSlot(void) {
            // Handle click event
        }
        void onTriggeredSlot(bool status) {
            // Handle triggered event
        }
        void onValuesSlot(int i, float j, int k) {
            // Handle values event
        }

    public:
        EVENT_SLOT(onClick);                      // Slot 'onClick' without any parameters
        EVENT_SLOT(onTriggered, bool);            // Slot 'onTriggered' with one parameter
        EVENT_SLOT(onValues, int, float, int);    // Slot 'onValues' with three parameters
};
```

## Connect Signals to Slots

It is possible to connect Signals to Slots, but also Signals to Signals. The Signal-to-Signal connection is a good solution in the case of subclasses. There is one macro which can be used to connect Signals and Slots or Signals.

```c++
EVENT_CONNECT(signal, slot_or_signal);
```

Example: Connect Signal to Slot

```c++
#include "Events.h"

Base sender;
Base receiver;

void setup(void)
{
    EVENT_CONNECT(sender.m_click_signal, receiver.m_onClick_slot);
}
```

## Emit a Signal

To send data between signal and slot, there are two possibilities. Each signal includes an `emit` method. This method has to be used to emit signals and depends on the number of defined arguments of the Signal. The second possibility is to use a macro.

Example: Emit Signals

```c++
#include "Events.h"

Base sender;
Base receiver;

void loop()
{
    sender.m_click_signal.emit();
    EVENT_EMIT(sender.m_click_signal);

    sender.m_triggered_signal.emit(true);
    EVENT_EMIT(sender.m_triggered_signal, true);

    sender.m_values_signal.emit(42, 3.14f, 42);
    EVENT_EMIT(sender.m_values_signal, 42, 3.14f, 42);

    delay(2);
}
```

---

**Anmerkungen zu den Korrekturen:**

- **Rechtschreibfehler behoben**: Wörter wie "marcos" wurden zu "macros" korrigiert, und "flot" wurde zu "float" korrigiert.
- **Inkonsequente Makro-Namen**: Die Makro-Namen wurden vereinheitlicht. Beispielsweise wurde `EVENT_REG_FOR_SLOTS()` zu `EVENT_REGISTER_EVENTS()` korrigiert, um konsistent mit der eigentlichen Makro-Definition zu sein.
- **Parameter- und Variablennamen korrigiert**: Schreibfehler in Variablennamen wie `m_onTriggerd_slot` wurden zu `m_onTriggered_slot` korrigiert.
- **Code-Beispiele aktualisiert**: Die Code-Beispiele wurden angepasst, um korrekte Syntax und Konsistenz zu gewährleisten. Beispielsweise wurde sichergestellt, dass bei der Verwendung von `emit` das Signal und nicht der Slot verwendet wird.
- **Syntaxfehler behoben**: Fehlende Semikolons und Klammern wurden hinzugefügt, um gültigen C++-Code sicherzustellen.
- **Klarheit und Konsistenz**: Beschreibungen und Kommentare wurden angepasst, um klarer und konsistenter zu sein.

Durch diese Anpassungen sollte das ReadMe verständlicher und frei von Fehlern sein, sodass es für Nutzer der Bibliothek einfacher ist, sie korrekt einzusetzen.