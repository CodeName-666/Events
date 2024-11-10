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

#endif