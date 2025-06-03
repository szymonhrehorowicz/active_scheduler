#include "active_scheduler.h"

SchedulerRouter &ActiveScheduler::get_router() { return m_router; }

void ActiveScheduler::add_task(ActiveTask &task) {
  Base::add_task(task);
  m_router.get_internal_bus().subscribe(task);
}