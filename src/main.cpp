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
    : public Active_Router<10, Motor_Control_Router, IncrementWorkMessage,
                           DecrementWorkMessage> {
public:
  Motor_Control_Router(Motor_Control_Active_Object &active_object)
      : m_active_object(active_object) {}

  void on_receive(const IncrementWorkMessage &);

  void on_receive(const DecrementWorkMessage &);

private:
  Motor_Control_Active_Object &m_active_object;
};

class Motor_Control_Active_Object : public Active_Object {
public:
  Motor_Control_Active_Object(uint32_t priority)
      : m_active_router(*this), Active_Object(priority, m_active_router) {};

  uint32_t task_request_work() const override { return m_work; }
  void task_process_work() override { --m_work; }

  void change_work_amount(int amount) {
    if ((m_work - amount) > 0) {
      m_work += amount;
    }
  }

private:
  Motor_Control_Router m_active_router;

  uint32_t m_work;
};

void Motor_Control_Router::on_receive(const IncrementWorkMessage &) {
  std::cout << "Incrementing work" << std::endl;
  m_active_object.change_work_amount(1);
}

void Motor_Control_Router::on_receive(const DecrementWorkMessage &) {
  std::cout << "Decrementing work" << std::endl;
  m_active_object.change_work_amount(-1);
}

int main() {
  etl::message_bus<2> public_bus;

  // Interrupts mock
  std::thread input_monitor(input_thread, std::ref(public_bus));
  input_monitor.detach();

  return 0;
}