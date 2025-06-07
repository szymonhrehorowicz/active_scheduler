#include "etl/message_bus.h"
#include "etl/scheduler.h"
#include "interrupt.h"
#include <cassert>
#include <thread>

#include "motor_control_ao.h"
#include "motor_control_router.h"

#include "pedal_position_ao.h"
#include "pedal_position_router.h"

int main() {
  etl::message_bus<2> public_bus;

  Motor_Control_Active_Object motor_control_task{1, 1, public_bus};
  Pedal_Position_Active_Object pedal_position_task{2, 2, public_bus};

  etl::scheduler<etl::scheduler_policy_highest_priority, 2> scheduler;

  public_bus.subscribe(motor_control_task.get_internal_router());
  public_bus.subscribe(pedal_position_task.get_internal_router());

  scheduler.add_task(motor_control_task);
  scheduler.add_task(pedal_position_task);

  // Interrupts mock
  std::thread input_monitor(input_thread, std::ref(public_bus));
  input_monitor.detach();

  scheduler.start();

  return 0;
}