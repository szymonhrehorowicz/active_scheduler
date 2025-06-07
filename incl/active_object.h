#pragma once

#include "active_router.h"
#include "etl/message_bus.h"
#include "etl/task.h"
#include <cassert>

class Active_Object_Interface : public etl::task {
public:
  Active_Object_Interface(uint32_t priority) : task(priority) {};
  ~Active_Object_Interface() = default;

  virtual uint32_t task_request_work() const = 0;
  virtual void task_process_work() = 0;
  virtual Active_Router_Interface &get_internal_router() = 0;

protected:
  virtual void set_internal_router(Active_Router_Interface &router) = 0;
};

class Active_Object : public Active_Object_Interface {
public:
  explicit Active_Object(uint32_t priority, etl::imessage_bus &public_bus)
      : Active_Object_Interface(priority), m_internal_router(nullptr),
        m_public_bus(public_bus) {}

  uint32_t task_request_work() const override;
  void task_process_work() override;
  Active_Router_Interface &get_internal_router() override;

  ~Active_Object();

protected:
  void set_internal_router(Active_Router_Interface &router) override;
  Active_Router_Interface *m_internal_router;

  etl::imessage_bus &m_public_bus;
};