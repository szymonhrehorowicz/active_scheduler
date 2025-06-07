#pragma once

#include "active_router.h"
#include "etl/message_bus.h"
#include "etl/task.h"
#include <cassert>

class Active_Object_Interface : public etl::task {
public:
  Active_Object_Interface(uint32_t priority) : task(priority) {};
  virtual ~Active_Object_Interface() = default;

  virtual uint32_t task_request_work() const = 0;
  virtual void task_process_work() = 0;
  virtual Active_Router_Interface &get_internal_router() = 0;
};

template <typename RouterType>
class Active_Object : public Active_Object_Interface {
  static_assert(std::is_base_of_v<Active_Router_Interface, RouterType>,
                "RouterType must inherit from Active_Router_Interface");

public:
  Active_Object(uint32_t priority, etl::message_router_id_t router_id,
                etl::imessage_bus &public_bus)
      : Active_Object_Interface(priority), m_router(router_id, *this),
        m_public_bus(public_bus) {}

  uint32_t task_request_work() const override {
    return m_router.get_queue_size();
  }

  void task_process_work() override { m_router.process_queue(); }

  Active_Router_Interface &get_internal_router() override { return m_router; }

protected:
  RouterType m_router;
  etl::imessage_bus &m_public_bus;
};