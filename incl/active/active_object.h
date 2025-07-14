#pragma once

#include "active_router.h"
#include "etl/message_bus.h"
#include "etl/task.h"
#include <cassert>

namespace Active
{

class Object_Interface : public etl::task
{
  public:
    Object_Interface(uint32_t priority) : task(priority) {};
    virtual ~Object_Interface() = default;

    virtual uint32_t task_request_work() const = 0;
    virtual void task_process_work() = 0;
    virtual Router_Interface &get_internal_router() = 0;
    virtual etl::imessage_bus &get_public_bus() = 0;
};

template <typename RouterType> class Object : public Object_Interface
{
    static_assert(std::is_base_of_v<Router_Interface, RouterType>, "RouterType must inherit from Router_Interface");

  public:
    Object(uint32_t priority, etl::message_router_id_t router_id, etl::imessage_bus &public_bus)
        : Object_Interface(priority), m_router(router_id, *this), m_public_bus(public_bus)
    {
    }

    virtual uint32_t task_request_work() const override
    {
        return m_router.get_queue_size();
    }

    virtual void task_process_work() override
    {
        m_router.process_queue();
    }

    Router_Interface &get_internal_router() override
    {
        return m_router;
    }

    etl::imessage_bus &get_public_bus() override
    {
        return m_public_bus;
    }

  protected:
    RouterType m_router;
    etl::imessage_bus &m_public_bus;
};

} // namespace Active
