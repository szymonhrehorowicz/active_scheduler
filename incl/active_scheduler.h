#pragma once

#include "active_task.h"
#include "etl/scheduler.h"
#include "scheduler_router.h"

class ActiveScheduler
    : public etl::scheduler<etl::scheduler_policy_sequential_single,
                            NUM_OF_TASKS> {
public:
  using Base =
      etl::scheduler<etl::scheduler_policy_sequential_single, NUM_OF_TASKS>;

  SchedulerRouter &get_router();

  void add_task(ActiveTask &task);

private:
  SchedulerRouter m_router{0};
};