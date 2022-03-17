#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>
#include <vector>

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
#define EVENT_SLOT(__name__, __type__)                                                 \
       EventSlot<__type__> m_##__name__##_slot{this , &ThisEventClass::__name__##Slot} \

/**
 * @brief Global Macro defintion for classes to register for slots
 */
#define EVENT_REG_FOR_SLOTS(__CLASS_NAME__)                                 \
        template <class T> using EventSlot = MethodSlot<__CLASS_NAME__, T>; \
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
template <class ParameterType> class Signal;

/**
 * @brief Global definition of the type EventSignal
 * @tparam T Datatype of the EventSignal
 */
template <class T> using EventSignal = Signal<T>;


/**
 * @brief Enumerate to identify SlotType
 */
enum SlotType { Function, Method };


/**
 * @brief Slot Template Class 
 * @tparam ParameterType Type of the Slot
 * 
 * The Slot base class, its template parameter indicates the datatype of the parameters it expects to receive. Slots can only
 * be connected to Signals with identical ParameterTypes.
 */
template <class ParameterType> class Slot {
    protected:
        Slot(SlotType slotType) : m_slot_type(slotType) { }

    public:
        /**
         * @brief Destroy the Slot object
         */
        virtual ~Slot() { }

        /**
         * @brief 
         * @param param 
         * Allows the slot to be called by the signal during firing.
         */
        virtual void operator()(ParameterType param) const = 0;

        /**
         * @brief 
         * @param slot 
         * @return true 
         * @return false 
         * 
         * Allows the slot to be removed via comparison.
         */
        virtual bool operator==(const Slot<ParameterType>* slot) const = 0;

        /**
         * @brief 
         * @return SlotType 
         * 
         * Provides roughly the same mechanism as RTTI.
         */
        SlotType slotType() const {
            return m_slot_type;
        }
        
    protected:
        SlotType m_slot_type; /*!< */
        
};

/**
 * @brief Signal Main Class
 * @tparam ParameterType 
 * 
 * The Signal class, we can implant these into ends and allow means to connect their members to them should they want to
 * receive callbacks from their children means. Ofcourse it's possible that these callbacks are made within the context of
 * an interrupt so the receipient will want to be fairly quick about how they process it.
 */
template <class ParameterType> class Signal {

    public:
    /**
     * @brief Construct a new Signal object 
     */
    Signal(uint16_t max_connections = 1) : m_max_onnections(max_connections), 
                                         m_size_of_connections(0) 
    { 
        /*All list are initiated whith null to reduce RAM usage*/
        m_connected_signals = nullptr; 
        m_connected_slots = nullptr;
    }

    /**
     * @brief Destroy the Signal object
     * 
     * Since the signal takes copies of all the input slots via clone() it needs to clean up after itself when being destroyed.
     */
    virtual ~Signal() {
        m_connected_slots->clear();
        m_connected_signals->clear();
    }

    /**
     * @brief Connect Signal
     * @param[in] signal Siganl to connect
     * 
     * Adds signal to list of connections.
     */
    void connect(const Signal<ParameterType>& signal) {
        /*Create Signal list for when the first Signal was attached*/
        if(m_connected_signals == nullptr)
        {
            m_connected_signals = new std::vector<Signal<ParameterType>*>();
        }

        if(m_size_of_connections < m_max_onnections)
        {
            m_connected_signals->push_back((Signal<ParameterType>*)&signal);
            m_size_of_connections++;
        }
    }

    /**
     * @brief Connect Slot
     * @param[in] slot Slot to connect
     * 
     * Adds a slot to list of connections.
     */
    void connect(const Slot<ParameterType>& slot) {
        if(m_connected_slots == nullptr)
        {
            m_connected_slots = new std::vector<Slot<ParameterType>*>();
        }

        if(m_size_of_connections < m_max_onnections)
        {
            m_connected_slots->push_back((Slot<ParameterType>*)&slot);
            m_size_of_connections++;
        }
    }

    /**
     * @brief Disconnect Slot
     * @param[in] slot Slot to disconnect
     * 
     * Removes slot from list of connections.
     */
    void disconnect(const Slot<ParameterType>& slot) {
        bool found = false;

        if(m_connected_slots != nullptr)
        {
            for (int i = 0; i < m_connected_slots->size() && found == false; i++)
            {
                if(&slot == m_connected_slots->at(i))
                {
                    m_connected_slots->erase(i);
                    m_size_of_connections--;
                    found = true;
                }
            }
        }
    }

    /**
     * @brief Disconnect a Signal
     * @param[in] signal Signal reference to disconnect
     * 
     * Removes signal from list of connections.
     */
    void disconnect(const Signal<ParameterType>& signal) {
        bool found = false;
        if(m_connected_signals != nullptr)
        {
            for (int i = 0; i < m_connected_signals->size() && found == false; i++)
            {
                if(&signal == m_connected_signals->at(i))
                {
                    m_connected_signals->erase(i);
                    m_size_of_connections--;
                    found = true;
                }
            }
        }
    }

    /**
     * @brief
     * @param param 
     * 
     * Visits each of its listeners and executes them via operator().
     */
    void emit(ParameterType param) const {
        if( m_connected_slots != nullptr)
        {
            for(Slot<ParameterType>* conSlot : *m_connected_slots)
            {
                (*conSlot)(param);
            }
        }

        if( m_connected_signals != nullptr)
        {
            for(Signal<ParameterType>* conSignal: *m_connected_signals)
            {
                conSignal->emit(param);
            }
        }    
    }

    /**
     * @brief 
     * @return uint16_t 
     */

    uint16_t connections(void)
    {
        return m_size_of_connections;
    }

    private:
        uint16_t m_max_onnections; /*!< */
        uint16_t m_size_of_connections; /*!< */
        std::vector<Slot<ParameterType>*> *m_connected_slots; /*!< */
        std::vector<Signal<ParameterType>*> *m_connected_signals; /*!< */
    

};

/**
 * @brief 
 * 
 * @tparam ParameterType 
 * 
 * FunctionSlot is a subclass of Slot for use with function pointers. In truth there's not really any need to wrap up free
 * standing function pointers into slots since any function in C/C++ is happy to accept a raw function pointer and execute it.
 * However this system allows free standing functions to be used alongside member functions or even arbitrary functor objects.
 */
template <class ParameterType> class FunctionSlot : public Slot<ParameterType> {
    
    typedef void (*FunctPtr)(ParameterType);
    
    public:
    /**
     * @brief Construct a new Function Slot object
     * @param funct 
     */
    FunctionSlot(FunctPtr funct = nullptr) : Slot<ParameterType>(Function), m_funct(funct) { }

    
    /**
     * @brief 
     * @param param 
     * 
     * Execute the slot.
     */
    void operator() (ParameterType param) const {
        if(m_funct != nullptr)
        {
            return (m_funct)(param);
        }
    }

    
    /**
     * @brief 
     * @param slot 
     * @return true 
     * @return false 
     * 
     * Compares the slot.
     */
    bool operator==(const Slot<ParameterType>* slot) const {
        if (slot && slot->slotType() == Slot<ParameterType>::m_slot_type) {
            const FunctionSlot<ParameterType>* functSlot = reinterpret_cast<const FunctionSlot<ParameterType>*>(slot);
            return functSlot && functSlot->m_funct == m_funct;
        }
        return false;
    }
        
    private:

    FunctPtr m_funct; /*!<A free standing function pointer. */
};

/**
 * @brief 
 * @tparam ObjectType 
 * @tparam ParameterType 
 * 
 * MethodSlot is a subclass of Slot that allows member function pointers to be used as slots. While free standing
 * pointers to functions are relatively intuitive here, Members functions need an additional template parameter, the
 * owner object type and they are executed via the ->* operator.
 */
template <class ObjectType, class ParameterType> class MethodSlot : public Slot<ParameterType> {
    
    typedef void (ObjectType::*FunctPtr)(ParameterType);

    public:
    /**
     * @brief Construct a new Method Slot object
     * @param obj 
     * @param funct 
     */
    MethodSlot(ObjectType *obj = nullptr, FunctPtr funct = nullptr) : Slot<ParameterType>(Method), m_obj(obj), m_funct(funct) { }
    
    /**
     * @brief Set the Method object
     * @param obj 
     * @param funct 
     */
    void setMethod(ObjectType *obj = nullptr, FunctPtr funct = nullptr) {
        m_obj = obj;
        m_funct = funct;
    }

    /**
     * @brief 
     * @param param 
     * 
     * Execute the slot.
     */
    void operator() (ParameterType param) const {
        if(m_obj != nullptr)
        {
            return (m_obj->*m_funct)(param);
        }
    }

    
    /**
     * @brief 
     * @param slot 
     * @return true 
     * @return false 
     * 
     * Compare the slot.
     */
    bool operator==(const Slot<ParameterType>* slot) const {
        if (slot && slot->slotType() == Slot<ParameterType>::m_slot_type) {
            const MethodSlot<ObjectType, ParameterType>* methSlot = reinterpret_cast<const MethodSlot<ObjectType, ParameterType>*>(slot);
            return methSlot && methSlot->m_obj == m_obj && methSlot->m_funct == m_funct;
        }
        return false;
    }

    private:
    ObjectType *m_obj;  /*!< The function pointer's owner object. */
    FunctPtr m_funct;   /*!< A function-pointer-to-method of class ObjectType. */
};

#endif // CALLBACK_H
