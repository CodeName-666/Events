#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>
#include <vector>
#include "EventSignal.h"
#include "EventSlot.h"


/**
 * @brief Global Macro definiton for classes to declare Sginals/Slots public.
 * 
 * This is a makro which makes it easier to differ between "normal" code 
 * and signals and slots.
 */
#define EVENT_PUBLIC public

/**
 * @brief Global Macro definiton for classes to declare Sginals/Slots private.
 * 
 * This is a makro which makes it easier to differ between "normal" code 
 * and signals and slots.
 */
#define EVENT_PRIVATE private

/**
 * @brief Global Macro definition to create a Event Slot with Method.
 * @param[in] __name__ Name of the Slot
 * @param[in] __type__ Type of the Slot
 */
#define EVENT_PARAM_SLOT(__name__, ...)                                                 \
       EventSlot<__VA_ARGS__> m_##__name__##_slot{this , &ThisEventClass::__name__##Slot} \


/**
 * @brief Global Macro definition to create a Event Slot with Method.
 * @param[in] __name__ Name of the Slot
 */
#define EVENT_VOID_SLOT(__name__)                                                 \
       EventSlot<> m_##__name__##_slot{this , &ThisEventClass::__name__##Slot} \



/**
 * @brief Global Macro defintion for classes to register for slots
 */
#define EVENT_REG_FOR_SLOTS(__CLASS_NAME__)                                 \
        template <class... T> using EventSlot = MethodSlot<__CLASS_NAME__, T...>; \
        using ThisEventClass = __CLASS_NAME__;

/**
 * @brief Global Macro definiton of Event Signal 2 Signal connector.
 */
#define EVENT_CONNECT_SIGNAL2SIGNAL(SIG_SOURCE,SIG_DEST)        \
    do                                                          \
    {                                                           \
        if((&SIG_SOURCE != nullptr) && (&SIG_DEST != nullptr))  \
        {                                                       \
            SIG_SOURCE.connect(SIG_DEST);                       \
        }                                                       \
                                                                \
    }while(0)                                                       


/**
 * @brief Global Macro definiton of Event Signal 2 Slot connector
 */
#define EVENT_CONNECT_SIGNAL2SLOT(SIGNAL,SLOT)           \
    do                                                   \
    {                                                    \
        if((&SIGNAL != nullptr) && (&SLOT != nullptr))   \
        {                                                \
            SIGNAL.connect(SLOT);                        \
        }                                                \
    }while(0)    


/**
 * @brief Global Macro definiton of Event Signal emitter
 */
#define EVENT_EMIT(SIGNAL,VALUE)  \
    do                            \
    {                             \
        if((&SIGNAL != nullptr))  \
        {                         \
            SIGNAL.emit(VALUE);   \
        }                         \
    }while(0) 

/**
 * @brief Forward decleartion of the clas Signal
 * @tparam ParameterType Type of the Signal
 */
template <class... Type> class Signal;

/**
 * @brief Global definition of the type EventSignal
 * @tparam T Datatype of the EventSignal
 */
template <class... T> using EventSignal = Signal<T...>;








#endif // CALLBACK_H
