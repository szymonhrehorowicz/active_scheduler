#include "active_scheduler.h"

ActiveScheduler::ActiveScheduler() : callback(*this) {
  set_idle_callback(callback);
}

void ActiveScheduler::add_task(Active_Object_Interface &task) {
  Base::add_task(task);
}