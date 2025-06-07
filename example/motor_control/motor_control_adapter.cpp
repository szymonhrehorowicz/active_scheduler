#include "motor_control_adapter.h"
#include "motor_control_task.h"

Motor_Control_Task &Motor_Control_Adapter::get_task() {
  return static_cast<Motor_Control_Task &>(m_owner);
}

void Motor_Control_Adapter::on_receive(const Tick_1ms_Message &) {
  auto &active_object = static_cast<Motor_Control_Task &>(m_owner);
  get_task().update_1ms();
}

void Motor_Control_Adapter::on_receive(
    const Steering_Wheel_Update_Message &message) {
  get_task().update_steering_wheel_data(message.angle);
}

void Motor_Control_Adapter::on_receive(
    const Pedal_Position_Update_Message &message) {
  get_task().update_pedal_position_data(message.position);
}