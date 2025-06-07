#include "active_router.h"

template <uint32_t QueueSize, typename TDerived, typename... TMessageTypes>
void Active_Router<QueueSize, TDerived, TMessageTypes...>::receive(
    const etl::imessage &msg) {
  if (accepts(msg)) {
    m_queue.emplace(msg);
  }
};

template <uint32_t QueueSize, typename TDerived, typename... TMessageTypes>
void Active_Router<QueueSize, TDerived, TMessageTypes...>::process_queue() {
  message_packet packet;

  while (m_queue.pop(packet)) {
    etl::imessage &msg = packet.get();
    Base::receive(msg);
  }
};
