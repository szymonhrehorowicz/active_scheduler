#include "motor_control_router.h"
#include "motor_control_ao.h"

void Motor_Control_Router::set_active_object(Motor_Control_Active_Object &obj) {
  assert(m_active_object == nullptr);
  m_active_object = &obj;
}

void Motor_Control_Router::on_receive(const Tick_1ms_Message &) {
  assert(m_active_object != nullptr);
  m_active_object->update_1ms();
}

void Motor_Control_Router::on_receive(
    const Steering_Wheel_Update_Message &message) {
  assert(m_active_object != nullptr);
  m_active_object->update_steering_wheel_data(message.angle);
}

void Motor_Control_Router::on_receive(
    const Pedal_Position_Update_Message &message) {
  assert(m_active_object != nullptr);
  m_active_object->update_pedal_position_data(message.position);
}