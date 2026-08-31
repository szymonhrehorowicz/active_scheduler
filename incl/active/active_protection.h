#pragma once

#include "active/active_message.h"
#include "active/active_router.h"
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
    FC1 = 2,
    Reset = 3
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
using Protection_Reset = Active::Message<Protection_Events::Reset>;

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

template <uint32_t Code> class Protection : public etl::message_router<Protection<Code>, Protection_Reset>
{
    using Base = etl::message_router<Protection<Code>, Protection_Reset>;

  public:
    explicit Protection(etl::imessage_bus &protection_bus, Protection_Events level, Protection_Function function)
        : Base(Code), m_protection_bus(protection_bus), m_protection_level(level), m_function(std::move(function))
    {
        protection_bus.subscribe(*this);
    };

    void on_receive(Protection_Reset const & /*message*/)
    {
        std::cout << "Protection " << Code << " has been reset\n";
        m_has_raised_fault = false;
    }

    void on_receive_unknown(const etl::imessage & /*msg*/)
    {
        assert(false && "Protection received unknown message");
    }

    void evaluate()
    {
        auto const result = m_function();

        if (!m_has_raised_fault && result.is_triggered)
        {
            Protection_Event_Data event_data{.code = Code, .aux_code = result.aux_code};
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
    etl::imessage_bus &m_protection_bus;
    Protection_Function m_function;
    Protection_Events m_protection_level;
    bool m_has_raised_fault{false};
};

namespace internal
{
class Protection_Manager_Impl
    : public etl::message_router<Protection_Manager_Impl, Protection_Alarm, Protection_FC2, Protection_FC1>
{
    using Base = etl::message_router<Protection_Manager_Impl, Protection_Alarm, Protection_FC2, Protection_FC1>;

  public:
    Protection_Manager_Impl() : Base(0)
    {
        m_protection_bus.subscribe(*this);
    };

    void on_receive(Protection_Alarm const &event)
    {
        auto const payload = etl::get<0>(event.get_payload());
        std::cout << "Alarm [code: " << payload.code << " | aux code: " << payload.aux_code << "]\n";
        m_protection_bus.receive(Protection_Reset{});
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

    etl::imessage_bus &get_bus()
    {
        return m_protection_bus;
    }

  private:
    // Note: There could be multiple buses, one for timelevel. Factory methods would create protections passing them
    // correct (timelevel-wise) bus.
    etl::message_bus<100> m_protection_bus{};
};
} // namespace internal

using Protection_Manager = etl::singleton<internal::Protection_Manager_Impl>;

} // namespace Active