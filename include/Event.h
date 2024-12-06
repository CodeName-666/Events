#ifndef EVENT_H
#define EVENT_H

#include "Event_Config.h"

#if (EVENT_USE_CPP_DEV_TYPE == EVENT_TYPE_EMBEDDED)
    #include "EventSignalAv.h"
#elif (EVENT_USE_CPP_DEV_TYPE == EVENT_TYPE_STANDARD)
    #include "EventSignalLst.h"
#else 
    #error "!!!!Invlaid EVENT_USE_CPP_DEV_TYPE set!!!!"
#endif

#include "EventSlot.h"


#define CONCAT(a, b, c) a##b##c

#define EXPAND_AND_CONCAT(a, b, c) CONCAT(a, b, c)  


#define EVENT_RESOLVE_SIGNAL(__name__) \
    EXPAND_AND_CONCAT(EVENT_SIGNAL_PREFIX , __name__, EVENT_SIGNAL_SUFFIX)

#define EVENT_RESOLVE_SLOT(__name__) \

#define EVENT_SIGNAL(__name__,args...)                                              \
    EventSignal<args> EXPAND_AND_CONCAT(EVENT_SIGNAL_PREFIX , __name__, EVENT_SIGNAL_SUFFIX)

/**
 * @brief Global Macro definition to create a Event Slot with Method.
 * @param[in] __name__ Name of the Slot
 * @param[in] __type__ Type of the Slot
 */
#define EVENT_SLOT(__name__,args...)                                               \
       EventSlot<args> EXPAND_AND_CONCAT(EVENT_SLOT_PREFIX, __name__, EVENT_SLOT_SUFFIX) {this , &ThisEventClass::EXPAND_AND_CONCAT(EVENT_SLOT_METHOD_PREFIX, __name__, EVENT_SLOT_METHOD_SUFFIX)}      
    


#define EVENT_FNC_SLOT(__name__,args...)                                            \
        EventFncSlot<args> EXPAND_AND_CONCAT(EVENT_SLOT_PREFIX, __name__ , EVENT_SLOT_SUFFIX) {EXPAND_AND_CONCAT(EVENT_SLOT_METHOD_PREFIX, __name__, EVENT_SLOT_METHOD_SUFFIX)}
/**
 * @brief Global Macro definiton of Event Signal 2 Slot connector
 */
#define EVENT_CONNECT(SIGNAL,SLOT_OR_SIGNAL)                       \
    do                                                             \
    {                                                              \
        if((&SIGNAL != nullptr) && (&SLOT_OR_SIGNAL != nullptr))   \
        {                                                          \
            SIGNAL.connect(SLOT_OR_SIGNAL);                        \
        }                                                          \
    }while(0)    

/**
 * @brief Global Macro defintion for classes to register for events and slots
 */
#define EVENT_REGISTER_EVENTS(__CLASS_NAME__)                                                \
        template <class... T> using EventSlot = MethodSlot<__CLASS_NAME__, T...>;            \
        using ThisEventClass = __CLASS_NAME__;                                               \
        public:                                                                              \
        template <class... Type>                                                             \
        static void connect(Signal<Type ...>& tx_signal, const Slot<Type ...>& rx_slot)      \
        {                                                                                    \
            tx_signal.connect(rx_slot);                                                      \
        }                                                                                    \
        template <class... Type>                                                             \
        static void connect(Signal<Type ...>& tx_signal, const Signal<Type ...>& rx_signal)  \
        {                                                                                    \
            tx_signal.connect(rx_signal);                                                    \
        }
 

/**
 * @brief Global Macro definiton of Event Signal emitter
 */
#define EVENT_EMIT(SIGNAL,args...)  \
    do                              \
    {                               \
        if((&(SIGNAL) != nullptr))    \
        {                           \
            (SIGNAL).emit(args);      \
        }                           \
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

/**
 * @brief 
 * 
 * @tparam T 
 */
template <class... T> using EventFncSlot = FunctionSlot<T...>;





#endif // CALLBACK_H
