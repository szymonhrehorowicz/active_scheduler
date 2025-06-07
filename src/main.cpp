#include "etl/message_bus.h"
#include "etl/scheduler.h"
#include "interrupt.h"
#include <cassert>
#include <iostream>
#include <thread>

#include "motor_control/motor_control_adapter.h"
#include "motor_control/motor_control_task.h"

#include "pedal_position/pedal_position_adapter.h"
#include "pedal_position/pedal_position_task.h"

int main() {
  etl::message_bus<2> public_bus;

  Motor_Control_Task motor_control_task{1, 1, public_bus};
  Pedal_Position_Task pedal_position_task{2, 2, public_bus};

  etl::scheduler<etl::scheduler_policy_highest_priority, 2> scheduler;

  public_bus.subscribe(motor_control_task.get_internal_router());
  public_bus.subscribe(pedal_position_task.get_internal_router());

  scheduler.add_task(motor_control_task);
  scheduler.add_task(pedal_position_task);

  // Interrupts mock
  std::cout << "Control interrupts from terminal, send 'h' for help"
            << std::endl;
  std::thread input_monitor(input_thread, std::ref(public_bus),
                            std::ref(scheduler));
  input_monitor.detach();

  scheduler.start();

  return 0;
}