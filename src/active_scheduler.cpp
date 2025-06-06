#include "active_scheduler.h"

ActiveScheduler::ActiveScheduler() : callback(*this) {
  set_idle_callback(callback);
}

SchedulerRouter &ActiveScheduler::get_router() { return m_router; }

void ActiveScheduler::add_task(ActiveTask &task) {
  Base::add_task(task);
  m_router.get_internal_bus().subscribe(task);
}

void ActiveScheduler::idle_callback() { m_router.process_queue(); };