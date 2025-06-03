#include "scheduler_router.h"
#include <iostream>

void SchedulerRouter::receive(const etl::imessage &msg) {
  if (accepts(msg)) {
    m_queue.emplace(msg);

    std::cout << "Queueing message " << int(msg.get_message_id()) << std::endl;
  } else {
    std::cout << "Ignoring message " << int(msg.get_message_id()) << std::endl;
  }
};

void SchedulerRouter::process_queue() {
  while (!m_queue.empty()) {
    message_packet &packet = m_queue.front();
    etl::imessage &msg = packet.get();
    std::cout << "Processing message " << int(msg.get_message_id())
              << std::endl;
    Base::receive(msg);
    m_queue.pop();
  }
};

void SchedulerRouter::on_receive(const IncrementWorkMessage &msg) {
  std::cout << "Router sending increment work message" << std::endl;
  m_internal_bus.receive(msg);
}

void SchedulerRouter::on_receive(const DecrementWorkMessage &msg) {
  std::cout << "Router sending decrement work message" << std::endl;
  m_internal_bus.receive(msg);
}

void SchedulerRouter::on_receive_unknown(const etl::imessage &msg) {
  std::cout << "  Received unknown message " << int(msg.get_message_id())
            << std::endl;
}

etl::imessage_bus &SchedulerRouter::get_internal_bus() {
  return m_internal_bus;
}