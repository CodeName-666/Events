#ifndef EVENTSIGNAL_H
#define EVENTSIGNAL_H

#include <stdint.h>
#include <vector>
#include "EventSlot.h"


/**
 * @brief Signal Main Class
 * @tparam Type
 *
 * The Signal class, we can implant these into ends and allow means to connect their members to them should they want to
 * receive callbacks from their children means. Ofcourse it's possible that these callbacks are made within the context of
 * an interrupt so the receipient will want to be fairly quick about how they process it.
 */
template <class... Type> class Signal {

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
    void connect(const Signal<Type ...>& signal) {
        /*Create Signal list for when the first Signal was attached*/
        if(m_connected_signals == nullptr)
        {
            m_connected_signals = new std::vector<Signal<Type ...>*>();
        }

        if(m_size_of_connections < m_max_onnections)
        {
            m_connected_signals->push_back((Signal<Type... >*)&signal);
            m_size_of_connections++;
        }
    }

    /**
     * @brief Connect Slot
     * @param[in] slot Slot to connect
     *
     * Adds a slot to list of connections.
     */
    void connect(const Slot<Type ...>& slot) {
        if(m_connected_slots == nullptr)
        {
            m_connected_slots = new std::vector<Slot<Type ...>*>();
        }

        if(m_size_of_connections < m_max_onnections)
        {
            m_connected_slots->push_back((Slot<Type ...>*)&slot);
            m_size_of_connections++;
        }
    }

    /**
     * @brief Disconnect Slot
     * @param[in] slot Slot to disconnect
     *
     * Removes slot from list of connections.
     */
    void disconnect(const Slot<Type ...>& slot) {
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
    void disconnect(const Signal<Type ...>& signal) {
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
    void emit(Type ... args) const {
        if( m_connected_slots != nullptr)
        {
            for(Slot<Type...>* conSlot : *m_connected_slots)
            {
                (*conSlot)(args...);
            }
        }

        if( m_connected_signals != nullptr)
        {
            for(Signal<Type ...>* conSignal: *m_connected_signals)
            {
                conSignal->emit(args...);
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
        std::vector<Slot<Type ...>*> *m_connected_slots; /*!< */
        std::vector<Signal<Type ...>*> *m_connected_signals; /*!< */


};




#endif // EVENTSIGNAL_H
