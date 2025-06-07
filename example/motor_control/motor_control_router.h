#pragma once

#include "active_router.h"
#include "messages.h"

class Motor_Control_Active_Object;

class Motor_Control_Router
    : public Active_Router<10, Motor_Control_Router, Tick_1ms_Message,
                           Steering_Wheel_Update_Message,
                           Pedal_Position_Update_Message> {
public:
  Motor_Control_Router(etl::message_router_id_t id,
                       Active_Object_Interface &owner)
      : Active_Router(id, owner) {}

  void on_receive(const Tick_1ms_Message &);
  void on_receive(const Steering_Wheel_Update_Message &);
  void on_receive(const Pedal_Position_Update_Message &);
};
