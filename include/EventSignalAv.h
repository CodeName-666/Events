#ifndef EVENTSIGNAL_H
#define EVENTSIGNAL_H

#include <stdint.h>
#ifdef ARDUINO_ARCH_AVR
   // #include "ArduinoSTL.h"
   // #include <vector>  // Entfernt, da wir Standardarrays verwenden
#else 
    // #include <vector>  // Entfernt, da wir Standardarrays verwenden
#endif
#include "EventSlot.h"

/**
 * @brief Signal-Hauptklasse
 * @tparam Type
 *
 * Die Signal-Klasse ermöglicht es, Slots und andere Signale zu verbinden, um Rückrufe zu erhalten.
 * Beachten Sie, dass diese Rückrufe im Kontext eines Interrupts erfolgen können, daher sollte die Verarbeitung schnell sein.
 */
template <class... Type> class Signal {

    public:
    /**
     * @brief Konstruktor für ein neues Signal-Objekt
     */
    Signal(uint16_t max_connections = UINT16_MAX) : m_max_connections(max_connections),
                                                    m_size_of_connections(0),
                                                    m_connected_slots(nullptr),
                                                    m_connected_signals(nullptr),
                                                    m_num_connected_slots(0),
                                                    m_num_connected_signals(0)
    {
        /* Alle Listen werden mit nullptr initialisiert, um den RAM-Verbrauch zu reduzieren */
    }

    /**
     * @brief Destruktor für das Signal-Objekt
     *
     * Da das Signal Kopien aller Eingangsslots über clone() nimmt, muss es beim Zerstören aufräumen.
     */
    virtual ~Signal() {
        if (m_connected_slots != nullptr) {
            delete[] m_connected_slots;
            m_connected_slots = nullptr;
        }
        if (m_connected_signals != nullptr) {
            delete[] m_connected_signals;
            m_connected_signals = nullptr;
        }
    }

    /**
     * @brief Verbinde ein anderes Signal
     * @param[in] signal Signal zum Verbinden
     *
     * Fügt ein Signal zur Liste der Verbindungen hinzu.
     */
    void connect(const Signal<Type ...>& signal) {
        /* Signal-Liste erstellen, wenn das erste Signal angehängt wird */
        if(m_connected_signals == nullptr)
        {
            m_connected_signals = new Signal<Type ...>*[m_max_connections];
            m_num_connected_signals = 0;
        }

        if(m_size_of_connections < m_max_connections)
        {
            m_connected_signals[m_num_connected_signals++] = (Signal<Type... >*)&signal;
            m_size_of_connections++;
        }
    }

    /**
     * @brief Verbinde einen Slot
     * @param[in] slot Slot zum Verbinden
     *
     * Fügt einen Slot zur Liste der Verbindungen hinzu.
     */
    void connect(const Slot<Type ...>& slot) {
        if(m_connected_slots == nullptr)
        {
            m_connected_slots = new Slot<Type ...>*[m_max_connections];
            m_num_connected_slots = 0;
        }

        if(m_size_of_connections < m_max_connections)
        {
            m_connected_slots[m_num_connected_slots++] = (Slot<Type ...>*)&slot;
            m_size_of_connections++;
        }
    }

    /**
     * @brief Trenne einen Slot
     * @param[in] slot Slot zum Trennen
     *
     * Entfernt einen Slot aus der Liste der Verbindungen.
     */
    void disconnect(const Slot<Type ...>& slot) {
        if(m_connected_slots != nullptr)
        {
            for (uint16_t i = 0; i < m_num_connected_slots; i++)
            {
                if(&slot == m_connected_slots[i])
                {
                    removeFromArray(m_connected_slots, m_num_connected_slots, i);
                    m_size_of_connections--;
                    break;
                }
            }
        }
    }

    /**
     * @brief Trenne ein Signal
     * @param[in] signal Signal zum Trennen
     *
     * Entfernt ein Signal aus der Liste der Verbindungen.
     */
    void disconnect(const Signal<Type ...>& signal) {
        if(m_connected_signals != nullptr)
        {
            for (uint16_t i = 0; i < m_num_connected_signals; i++)
            {
                if(&signal == m_connected_signals[i])
                {
                    removeFromArray(m_connected_signals, m_num_connected_signals, i);
                    m_size_of_connections--;
                    break;
                }
            }
        }
    }

    /**
     * @brief Emit-Funktion
     * @param args Parameter
     *
     * Ruft alle verbundenen Slots und Signale auf.
     */
    void emit(Type ... args) const {
        if( m_connected_slots != nullptr)
        {
            for(uint16_t i = 0; i < m_num_connected_slots; i++)
            {
                (*m_connected_slots[i])(args...);
            }
        }

        if( m_connected_signals != nullptr)
        {
            for(uint16_t i = 0; i < m_num_connected_signals; i++)
            {
                m_connected_signals[i]->emit(args...);
            }
        }
    }

    /**
     * @brief Gibt die Anzahl der Verbindungen zurück
     * @return uint16_t
     */
    uint16_t connections(void)
    {
        return m_size_of_connections;
    }

    private: /* Methods */ 
    
         /**
         * @brief Entfernt ein Element aus einem Array und verschiebt die restlichen Elemente nach links
         * @tparam T Typ des Arrays (Slot oder Signal)
         * @param array Zeiger auf das Array
         * @param num_elements Referenz auf die Anzahl der Elemente im Array
         * @param index_to_remove Index des zu entfernenden Elements
         */
        template <typename T>
        void removeFromArray(T** array, uint16_t& num_elements, uint16_t index_to_remove)
        {
            for (uint16_t j = index_to_remove; j < num_elements - 1; j++)
            {
                array[j] = array[j + 1];
            }
            num_elements--;
        }

    private: /*Parameter*/
        uint16_t m_max_connections; /*!< Maximale Anzahl von Verbindungen */
        uint16_t m_size_of_connections; /*!< Aktuelle Anzahl von Verbindungen */
        Slot<Type ...>** m_connected_slots; /*!< Array von Zeigern auf Slots */
        Signal<Type ...>** m_connected_signals; /*!< Array von Zeigern auf Signale */
        uint16_t m_num_connected_slots; /*!< Anzahl der verbundenen Slots */
        uint16_t m_num_connected_signals; /*!< Anzahl der verbundenen Signale */
};

#endif // EVENTSIGNAL_H
