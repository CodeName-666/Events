#ifndef EVENTSLOT_H
#define EVENTSLOT_H

#include <stdint.h>


/**
 * @brief Enumerate to identify SlotType
 */
enum SlotType { Function, Method };


/**
 * @brief Slot Template Class
 * @tparam Type Type of the Slot
 *
 * The Slot base class, its template parameter indicates the datatype of the parameters it expects to receive. Slots can only
 * be connected to Signals with identical Types.
 */
template <class... Type> class Slot {
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
        virtual void operator()(Type ...) const = 0;

        /**
         * @brief
         * @param slot
         * @return true
         * @return false
         *
         * Allows the slot to be removed via comparison.
         */
        virtual bool operator==(const Slot<Type ...>* slot) const = 0;

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
 * @brief
 *
 * @tparam Type
 *
 * FunctionSlot is a subclass of Slot for use with function pointers. In truth there's not really any need to wrap up free
 * standing function pointers into slots since any function in C/C++ is happy to accept a raw function pointer and execute it.
 * However this system allows free standing functions to be used alongside member functions or even arbitrary functor objects.
 */
template <class... Type> class FunctionSlot : public Slot<Type ...> {

    typedef void (*FunctPtr)(Type ...);

    public:
    /**
     * @brief Construct a new Function Slot object
     * @param funct
     */
    FunctionSlot(FunctPtr funct = nullptr) : Slot<Type ...>(Function), m_funct(funct) { }


    /**
     * @brief
     * @param param
     *
     * Execute the slot.
     */
    void operator() (Type... args) const {
        if(m_funct != nullptr)
        {
            return (m_funct)(args ...);
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
    bool operator==(const Slot<Type ...>* slot) const {
        if (slot && slot->slotType() == Slot<Type ...>::m_slot_type) {
            const FunctionSlot<Type ...>* functSlot = reinterpret_cast<const FunctionSlot<Type ...>*>(slot);
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
 * @tparam Type
 *
 * MethodSlot is a subclass of Slot that allows member function pointers to be used as slots. While free standing
 * pointers to functions are relatively intuitive here, Members functions need an additional template parameter, the
 * owner object type and they are executed via the ->* operator.
 */
template <class ObjectType, class... Type> class MethodSlot : public Slot<Type ...> {

    typedef void (ObjectType::*FunctPtr)(Type... args);

    public:
    /**
     * @brief Construct a new Method Slot object
     * @param obj
     * @param funct
     */
    MethodSlot(ObjectType *obj = nullptr, FunctPtr funct = nullptr) : Slot<Type... >(Method), m_obj(obj), m_funct(funct) { }

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
    void operator() (Type ... args) const {
        if(m_obj != nullptr)
        {
            return (m_obj->*m_funct)(args...);
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
    bool operator==(const Slot<Type ...>* slot) const {
        if (slot && slot->slotType() == Slot<Type ...>::m_slot_type) {
            const MethodSlot<ObjectType, Type ...>* methSlot = reinterpret_cast<const MethodSlot<ObjectType, Type ...>*>(slot);
            return methSlot && methSlot->m_obj == m_obj && methSlot->m_funct == m_funct;
        }
        return false;
    }

    private:
    ObjectType *m_obj;  /*!< The function pointer's owner object. */
    FunctPtr m_funct;   /*!< A function-pointer-to-method of class ObjectType. */
};


#endif // EVENTSLOT_H
