#include "motor_control_adapter.h"
#include "motor_control_task.h"
#include <etl/tuple.h>

Motor_Control_Task &Motor_Control_Adapter::get_task() {
  return static_cast<Motor_Control_Task &>(m_owner);
}

void Motor_Control_Adapter::on_receive(const Tick_1ms_Message &) {
  get_task().update_1ms();
}

void Motor_Control_Adapter::on_receive(
    const Steering_Wheel_Update_Message &message) {
  get_task().update_steering_wheel_data(etl::get<0>(message.get_payload()));
}

void Motor_Control_Adapter::on_receive(
    const Pedal_Position_Update_Message &message) {
  get_task().update_pedal_position_data(etl::get<0>(message.get_payload()));
}