#pragma once

#include "active_object.h"
#include "motor_control_router.h"
#include <string_view>

class Motor_Control_Active_Object : public Active_Object<Motor_Control_Router> {
public:
  static constexpr std::string_view CLASS_NAME{
      "Motor_Control_Active_Object | "};

  Motor_Control_Active_Object(uint32_t priority,
                              etl::message_router_id_t router_id,
                              etl::imessage_bus &public_bus)
      : Active_Object(priority, router_id, public_bus) {}

  void update_1ms();
  void update_steering_wheel_data(float angle);
  void update_pedal_position_data(float position);
};