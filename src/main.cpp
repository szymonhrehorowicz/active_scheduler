#include "active_object.h"
#include "active_router.h"
#include "etl/message_bus.h"
#include "etl/scheduler.h"
#include "etl/task.h"
#include "interrupt.h"
#include "messages.h"
#include <cstdint>
#include <iostream>
#include <thread>

class Motor_Control_Active_Object;

class Motor_Control_Router
    : public Active_Router<10, Motor_Control_Router, Tick_1ms_Message,
                           Steering_Wheel_Update_Message,
                           Pedal_Position_Update_Message> {
public:
  Motor_Control_Router(Motor_Control_Active_Object &active_object,
                       etl::message_router_id_t id)
      : Active_Router(id), m_active_object(active_object) {}

  void on_receive(const Tick_1ms_Message &);
  void on_receive(const Steering_Wheel_Update_Message &);
  void on_receive(const Pedal_Position_Update_Message &);

private:
  Motor_Control_Active_Object &m_active_object;
};

class Motor_Control_Active_Object : public Active_Object {
public:
  Motor_Control_Active_Object(uint32_t priority,
                              etl::message_router_id_t router_id)
      : m_router(*this, router_id), Active_Object(priority, m_router) {};

  void update_1ms() { std::cout << "Updating 1 ms" << std::endl; }

  void update_steering_wheel_data(float angle) {
    std::cout << "New steering wheel angle received: " << angle << std::endl;
  }

  void update_pedal_position_data(float position) {
    std::cout << "New pedal position received: " << position << std::endl;
  }

private:
  Motor_Control_Router m_router;

  uint32_t m_work{0};
};

void Motor_Control_Router::on_receive(const Tick_1ms_Message &) {
  m_active_object.update_1ms();
};

void Motor_Control_Router::on_receive(
    const Steering_Wheel_Update_Message &message) {
  m_active_object.update_steering_wheel_data(message.angle);
};

void Motor_Control_Router::on_receive(
    const Pedal_Position_Update_Message &message) {
  m_active_object.update_pedal_position_data(message.position);
};

int main() {
  etl::message_bus<2> public_bus;

  Motor_Control_Active_Object motor_control_task{1, 1};
  etl::scheduler<etl::scheduler_policy_sequential_single, 1> scheduler;

  public_bus.subscribe(motor_control_task.get_internal_router());

  scheduler.add_task(motor_control_task);

  // Interrupts mock
  std::thread input_monitor(input_thread, std::ref(public_bus));
  input_monitor.detach();

  scheduler.start();

  return 0;
}