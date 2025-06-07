#pragma once

#include "active_router.h"
#include "messages.h"

class Motor_Control_Active_Object;

class Motor_Control_Router
    : public Active_Router<10, Motor_Control_Router, Tick_1ms_Message,
                           Steering_Wheel_Update_Message,
                           Pedal_Position_Update_Message> {
public:
  Motor_Control_Router(etl::message_router_id_t id)
      : Active_Router(id), m_active_object(nullptr) {}

  void set_active_object(Motor_Control_Active_Object &obj);

  void on_receive(const Tick_1ms_Message &);
  void on_receive(const Steering_Wheel_Update_Message &);
  void on_receive(const Pedal_Position_Update_Message &);

private:
  Motor_Control_Active_Object *m_active_object;
};
