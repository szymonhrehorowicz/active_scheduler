#pragma once

#include "etl/message_router.h"
#include "etl/queue_spsc_atomic.h"

template <uint32_t QueueSize, typename TDerived, typename... TMessageTypes>
class Active_Router : public etl::message_router<TDerived, TMessageTypes...> {
public:
  using Base = etl::message_router<TDerived, TMessageTypes...>;
  using message_packet = etl::message_packet<TMessageTypes...>;

  void receive(const etl::imessage &msg) override;

  void process_queue();

  void on_receive_unknown(const etl::imessage &) {
    m_unknown_message_flag = true;
  }

  bool get_unknown_msg_flag() { return m_unknown_message_flag; }

  void reset_unknown_msg_flag() { m_unknown_message_flag = false; }

private:
  etl::queue_spsc_atomic<message_packet, QueueSize> m_queue;
  bool m_unknown_message_flag{};
};