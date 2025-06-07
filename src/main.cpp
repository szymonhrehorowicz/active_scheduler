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

// Forward declare router
class Motor_Control_Router;

class Motor_Control_Active_Object : public Active_Object {
private:
  uint32_t m_work{0};
  Motor_Control_Router
      *m_router; // Non-owning pointer since router references this object

public:
  Motor_Control_Active_Object(uint32_t priority);
  ~Motor_Control_Active_Object() = default;

  uint32_t task_request_work() const override { return m_work; }
  void task_process_work() override { --m_work; }

  void change_work_amount(int amount) {
    if ((m_work + amount) > 0) { // Fixed logical error in condition
      m_work += amount;
    }
  }
};

class Motor_Control_Router
    : public Active_Router<10, Motor_Control_Router, IncrementWorkMessage,
                           DecrementWorkMessage> {
public:
  explicit Motor_Control_Router(Motor_Control_Active_Object &active_object)
      : m_active_object(active_object) {}

  void on_receive(const IncrementWorkMessage &) {
    std::cout << "Incrementing work" << std::endl;
    m_active_object.change_work_amount(1);
  }

  void on_receive(const DecrementWorkMessage &) {
    std::cout << "Decrementing work" << std::endl;
    m_active_object.change_work_amount(-1);
  }

private:
  Motor_Control_Active_Object &m_active_object;
};

// Define constructor after both classes are declared
inline Motor_Control_Active_Object::Motor_Control_Active_Object(
    uint32_t priority)
    : Active_Object(
          priority,
          *new Motor_Control_Router(
              *this)) // Leak is ok since router lifetime matches active object
      ,
      m_router(static_cast<Motor_Control_Router *>(&get_router())) {}

int main() {
  etl::message_bus<2> public_bus;

  // Interrupts mock
  std::thread input_monitor(input_thread, std::ref(public_bus));
  input_monitor.detach();

  return 0;
}