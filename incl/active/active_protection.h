#pragma once

#include "active/active_message.h"
#include "active/active_router.h"
#include "active/active_signal.h"
#include "etl/message_bus.h"
#include "etl/singleton.h"
#include <concepts>
#include <functional>
#include <iostream>
#include <utility>

namespace Active
{

enum Protection_Events
{
    Alarm = 0,
    FC2 = 1,
    FC1 = 2
};

struct Protection_Event_Data
{
    uint32_t code;
    uint32_t aux_code;
};

struct Protection_Data
{
    bool is_triggered{false};
    uint32_t aux_code{0};
};

using Protection_Alarm = Active::Message<Protection_Events::Alarm, Protection_Event_Data>;
using Protection_FC2 = Active::Message<Protection_Events::FC2, Protection_Event_Data>;
using Protection_FC1 = Active::Message<Protection_Events::FC1, Protection_Event_Data>;

// Temporary event for testing
using Reset_Faults = Active::Message<255>;

using Protection_Function = std::function<Protection_Data(void)>;

template <typename T>
concept ProtectionConcept = requires(T t) {
    { t.evaluate() } -> std::same_as<void>;
};

class Protection_Object
{
  public:
    template <typename T>
        requires ProtectionConcept<T>
    Protection_Object(T &obj) : self(&obj), eval_fn([](void *p) { static_cast<T *>(p)->evaluate(); })
    {
    }

    void evaluate()
    {
        eval_fn(self);
    }

  private:
    void *self;
    void (*eval_fn)(void *);
};

class Protection
{
  public:
    explicit Protection(uint32_t code, etl::imessage_bus &protection_bus, Signal<> &reset_signal,
                        Protection_Events level, Protection_Function function)
        : m_code(code), m_protection_bus(protection_bus), m_protection_level(level), m_function(std::move(function))
    {
        reset_signal.connect(this, &Protection::reset);
    };

    void reset()
    {
        m_has_raised_fault = false;
    }

    void evaluate()
    {
        auto const result = m_function();

        if (!m_has_raised_fault && result.is_triggered)
        {
            Protection_Event_Data event_data{.code = m_code, .aux_code = result.aux_code};
            m_has_raised_fault = true;

            switch (m_protection_level)
            {
            case Protection_Events::Alarm: {
                m_protection_bus.receive(Protection_Alarm{event_data});
            }
            break;
            case Protection_Events::FC2: {
                m_protection_bus.receive(Protection_FC2{event_data});
            }
            break;
            case Protection_Events::FC1: {
                m_protection_bus.receive(Protection_FC1{event_data});
            }
            break;
            default:
                assert(false && "Unsupported protection level used");
                break;
            }
        }
    }

  private:
    uint32_t m_code;
    etl::imessage_bus &m_protection_bus;
    Protection_Function m_function;
    Protection_Events m_protection_level;
    bool m_has_raised_fault{false};
};

namespace internal
{
class Protection_Manager_Impl : public etl::message_router<Protection_Manager_Impl, Protection_Alarm, Protection_FC2,
                                                           Protection_FC1, Reset_Faults>
{
    using Base =
        etl::message_router<Protection_Manager_Impl, Protection_Alarm, Protection_FC2, Protection_FC1, Reset_Faults>;

  public:
    Protection_Manager_Impl(etl::imessage_bus &reset_bus /*temporary for testing purposes*/) : Base(0)
    {
        reset_bus.subscribe(*this);
        m_bus_t0.subscribe(*this);
        m_bus_t1.subscribe(*this);
    };

    void on_receive(Reset_Faults const & /*event*/)
    {
        std::cout << "Reseting faults\n";
        m_reset_signal.emit();
    };

    void on_receive(Protection_Alarm const &event)
    {
        auto const payload = etl::get<0>(event.get_payload());
        std::cout << "Alarm [code: " << payload.code << " | aux code: " << payload.aux_code << "]\n";
    };

    void on_receive(Protection_FC2 const &event)
    {
        auto const payload = etl::get<0>(event.get_payload());
        std::cout << "FC2 [code: " << payload.code << " | aux code: " << payload.aux_code << "]\n";
    };

    void on_receive(Protection_FC1 const &event)
    {
        auto const payload = etl::get<0>(event.get_payload());
        std::cout << "FC1 [code: " << payload.code << " | aux code: " << payload.aux_code << "]\n";
    };

    void on_receive_unknown(const etl::imessage & /*msg*/)
    {
        assert(false && "Protection_Manager received unknown message");
    }

    etl::imessage_bus &get_bus_t0()
    {
        return m_bus_t0;
    }

    etl::imessage_bus &get_bus_t1()
    {
        return m_bus_t1;
    }

    Signal<> &get_reset_signal()
    {
        return m_reset_signal;
    }

  private:
    etl::message_bus<1> m_bus_t0{};
    etl::message_bus<1> m_bus_t1{};

    // Note: Each timelevel should have it's own reset signal
    Signal<> m_reset_signal{};
};
} // namespace internal

using Protection_Manager = etl::singleton<internal::Protection_Manager_Impl>;

Protection make_protection_t0(uint32_t code, Protection_Events level, Protection_Function function);

} // namespace Active