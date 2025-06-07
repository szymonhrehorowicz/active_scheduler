#include "active_object.h"

uint32_t Active_Object::task_request_work() const {
  assert(m_internal_router != nullptr);
  return m_internal_router->get_queue_size();
};

void Active_Object::task_process_work() {
  assert(m_internal_router != nullptr);
  m_internal_router->process_queue();
};

Active_Router_Interface &Active_Object::get_internal_router() {
  assert(m_internal_router != nullptr);
  return *m_internal_router;
};

void Active_Object::set_internal_router(Active_Router_Interface &router) {
  assert(m_internal_router == nullptr);
  m_internal_router = &router;
};