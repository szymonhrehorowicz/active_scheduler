#include "active_task.h"
#include "etl/function.h"
#include "etl/message_router.h"
#include "interrupt.h"
#include <cstdint>
#include <etl/task.h>
#include <iostream>
#include <thread>

class SomeTask : public ActiveTask {
public:
  SomeTask(int priority, int id) : ActiveTask(priority, 0), m_id(id) {}

  uint32_t task_request_work() const override {
    std::cout << "Work left in task " << m_id << ": " << m_work << std::endl;
    return m_work;
  }

  void task_process_work() override {
    --m_work;
    std::cout << "Doing work from task " << m_id << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
  }

private:
  int m_id;
};

int main() {
  SomeTask some_task{1, 1};
  SomeTask some_other_task{2, 2};
  ActiveScheduler scheduler;
  IdleHandler idle_handler{scheduler};

  etl::function_mv<IdleHandler, &IdleHandler::idle_callback> idle_callback(
      idle_handler);

  // Start input monitoring thread
  std::thread input_monitor(input_thread, std::ref(scheduler));
  input_monitor.detach(); // Let it run independently

  scheduler.add_task(some_task);
  scheduler.add_task(some_other_task);
  scheduler.set_idle_callback(idle_callback);

  std::cout << "Press 'Q' to trigger interrupt\n";
  scheduler.start();

  turn_off_thread();

  return 0;
}