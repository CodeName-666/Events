#ifndef EVENT_CONFIG_H 
#define EVENT_CONFIG_H


#ifndef EVENT_MAX_SIGNAL_CONNECTIONS          
    #define EVENT_MAX_SIGNAL_CONNECTIONS        3u
#endif


#define EVENT_TYPE_EMBEDDED                     0u
#define EVENT_TYPE_STANDARD                     1u

#ifndef EVENT_USE_CPP_DEV_TYPE
    #define EVENT_USE_CPP_DEV_TYPE         EVENT_TYPE_EMBEDDED
#endif



#define EVENT_SIGNAL_PREFIX                 m_
#define EVENT_SIGNAL_SUFFIX                 _signal

#define EVENT_SLOT_PREFIX                   m_       
#define EVENT_SLOT_SUFFIX                   _slot

#define EVENT_SLOT_METHOD_PREFIX             
#define EVENT_SLOT_METHOD_SUFFIX            

#endif
