#include "motor_control_router.h"
#include "motor_control_ao.h"

void Motor_Control_Router::on_receive(const Tick_1ms_Message &) {
  auto &active_object = static_cast<Motor_Control_Active_Object &>(m_owner);
  active_object.update_1ms();
}

void Motor_Control_Router::on_receive(
    const Steering_Wheel_Update_Message &message) {
  auto &active_object = static_cast<Motor_Control_Active_Object &>(m_owner);
  active_object.update_steering_wheel_data(message.angle);
}

void Motor_Control_Router::on_receive(
    const Pedal_Position_Update_Message &message) {
  auto &active_object = static_cast<Motor_Control_Active_Object &>(m_owner);
  active_object.update_pedal_position_data(message.position);
}