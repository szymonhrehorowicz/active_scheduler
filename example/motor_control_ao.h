#pragma once

#include "active_object.h"
#include "motor_control_router.h"

class Motor_Control_Active_Object : public Active_Object {
public:
  Motor_Control_Active_Object(uint32_t priority,
                              etl::message_router_id_t router_id);

  ~Motor_Control_Active_Object();

  void update_1ms();

  void update_steering_wheel_data(float angle);
  void update_pedal_position_data(float position);

private:
  explicit Motor_Control_Active_Object(uint32_t priority)
      : Active_Object(priority), m_router(nullptr) {}

  Motor_Control_Router *m_router;
  uint32_t m_work{0};
};