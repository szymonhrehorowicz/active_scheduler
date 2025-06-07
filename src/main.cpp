#include "etl/message_bus.h"
#include "etl/scheduler.h"
#include "interrupt.h"
#include <cassert>
#include <thread>

#include "motor_control_ao.h"
#include "motor_control_router.h"

int main() {
  etl::message_bus<2> public_bus;

  Motor_Control_Active_Object motor_control_task{1, 1};

  etl::scheduler<etl::scheduler_policy_sequential_single, 1> scheduler;

  public_bus.subscribe(motor_control_task.get_internal_router());

  scheduler.add_task(motor_control_task);

  // Interrupts mock
  std::thread input_monitor(input_thread, std::ref(public_bus));
  input_monitor.detach();

  scheduler.start();

  return 0;
}