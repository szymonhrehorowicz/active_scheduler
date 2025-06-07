#pragma once

#include "active_object.h"
#include "etl/scheduler.h"
#include "scheduler_router.h"
#include <etl/function.h>

class ActiveScheduler
    : public etl::scheduler<etl::scheduler_policy_sequential_single,
                            NUM_OF_TASKS> {
public:
  using Base =
      etl::scheduler<etl::scheduler_policy_sequential_single, NUM_OF_TASKS>;

  ActiveScheduler();

  void add_task(Active_Object_Interface &task);

private:
  void idle_callback();
  etl::function_mv<ActiveScheduler, &ActiveScheduler::idle_callback> callback;
};