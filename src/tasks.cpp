#include "tasks.h"
#include <iostream>

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

void IdleHandler::idle_callback() {
  std::cout << "Being idle" << std::endl;
  m_scheduler.exit_scheduler();
};