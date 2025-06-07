#pragma once

#include "active_object.h"
#include <string>

class Motor_Control_Active_Object : public Active_Object {
public:
  const std::string CLASS_NAME{"Motor_Control_Active_Object | "};

  Motor_Control_Active_Object(uint32_t priority,
                              etl::message_router_id_t router_id,
                              etl::imessage_bus &public_bus);

  void update_1ms();

  void update_steering_wheel_data(float angle);
  void update_pedal_position_data(float position);
};