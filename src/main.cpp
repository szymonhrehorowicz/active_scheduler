#include "etl/function.h"
#include "etl/message_router.h"
#include "etl/scheduler.h"
#include "interrupt.h"
#include "tasks.h"
#include <cstdint>
#include <iostream>
#include <thread>

class SomeTask : public ActiveTask {
public:
  SomeTask(int priority) : ActiveTask(priority, 0) {}

  uint32_t task_request_work() const override {
    std::cout << "Work left: " << m_work << std::endl;
    return m_work;
  }

  void task_process_work() override {
    --m_work;
    std::cout << "Doing work from SomeTask" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
};

int main() {
  SomeTask some_task{1};
  ActiveScheduler scheduler;
  IdleHandler idle_handler{scheduler};

  etl::function_mv<IdleHandler, &IdleHandler::idle_callback> idle_callback(
      idle_handler);

  // Start input monitoring thread
  std::thread input_monitor(input_thread, std::ref(scheduler));
  input_monitor.detach(); // Let it run independently

  scheduler.add_task(some_task);
  scheduler.set_idle_callback(idle_callback);

  std::cout << "Press 'Q' to trigger interrupt\n";
  scheduler.start();

  turn_off_thread();

  return 0;
}