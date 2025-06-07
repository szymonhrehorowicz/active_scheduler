#include "motor_control_ao.h"
#include <iostream>

Motor_Control_Active_Object::Motor_Control_Active_Object(
    uint32_t priority, etl::message_router_id_t router_id)
    : Active_Object(priority) {
  auto *router = new Motor_Control_Router(router_id);
  assert(router != nullptr);

  router->set_active_object(*this);
  this->set_internal_router(*router);
  m_router = router;
};

Motor_Control_Active_Object::~Motor_Control_Active_Object() {
  delete m_router;
};

void Motor_Control_Active_Object::update_1ms() {
  std::cout << "Updating 1 ms" << std::endl;
};

void Motor_Control_Active_Object::update_steering_wheel_data(float angle) {
  std::cout << "New steering wheel angle received: " << angle << std::endl;
};

void Motor_Control_Active_Object::update_pedal_position_data(float position) {
  std::cout << "New pedal position received: " << position << std::endl;
};