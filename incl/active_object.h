#pragma once

#include "active_router.h"
#include "etl/task.h"

class Active_Object_Interface : public etl::task {
public:
  Active_Object_Interface(uint32_t priority) : task(priority) {};
  ~Active_Object_Interface() = default;

  virtual uint32_t task_request_work() const = 0;
  virtual void task_process_work() = 0;
  virtual Active_Router_Interface &get_internal_router() = 0;
};

class Active_Object : public Active_Object_Interface {
public:
  Active_Object(uint32_t priority, Active_Router_Interface &internal_router)
      : Active_Object_Interface(priority),
        m_internal_router(internal_router) {};

  uint32_t task_request_work() const override {
    return m_internal_router.get_queue_size();
  }

  void task_process_work() override { m_internal_router.process_queue(); }

  Active_Router_Interface &get_internal_router() override {
    return m_internal_router;
  };

private:
  Active_Router_Interface &m_internal_router;
};