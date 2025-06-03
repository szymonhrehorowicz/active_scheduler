#include "active_task.h"
#include "active_scheduler.h"
#include <iostream>

// ActiveTask
void ActiveTask::on_receive(const IncrementWorkMessage &) {
  std::cout << "Incrementing work" << std::endl;
  ++m_work;
}

void ActiveTask::on_receive(const DecrementWorkMessage &) {
  std::cout << "Decrementing work" << std::endl;
  --m_work;
}

void ActiveTask::on_receive_unknown(const etl::imessage &) {
  std::cout << "Received unknown message" << std::endl;
};

// IdleHandler
void IdleHandler::idle_callback() { m_scheduler.get_router().process_queue(); };