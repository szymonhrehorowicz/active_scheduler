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
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

private:
  int m_id;
};

int main() {
  SomeTask some_task{1, 1};
  SomeTask some_other_task{2, 2};
  ActiveScheduler scheduler{};

  std::thread input_monitor(input_thread, std::ref(scheduler));
  input_monitor.detach();

  scheduler.add_task(some_task);
  scheduler.add_task(some_other_task);

  std::cout << "Press 'Q' to quit\n";
  scheduler.start();
  turn_off_thread();

  return 0;
}