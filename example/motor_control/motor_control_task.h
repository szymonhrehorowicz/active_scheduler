#pragma once

#include "active/active_object.h"
#include "motor_control_adapter.h"
#include <string_view>

class Motor_Control_Task : public Active::Object<Motor_Control_Adapter> {

  using AO = Active::Object<Motor_Control_Adapter>;

public:
  static constexpr std::string_view CLASS_NAME{"Motor_Control_Task | "};

  Motor_Control_Task(uint32_t priority, etl::message_router_id_t router_id,
                     etl::imessage_bus &public_bus)
      : AO(priority, router_id, public_bus) {}

  void update_1ms();
  void update_steering_wheel_data(float angle);
  void update_pedal_position_data(float position);
};