#pragma once

#include "etl/message_router.h"
#include "etl/queue_spsc_atomic.h"

class Active_Router_Interface {
public:
  virtual void process_queue() = 0;
  virtual uint32_t get_queue_size() const = 0;
};

template <uint32_t QueueSize, typename TDerived, typename... TMessageTypes>
class Active_Router : public Active_Router_Interface,
                      public etl::message_router<TDerived, TMessageTypes...> {
public:
  using Base = etl::message_router<TDerived, TMessageTypes...>;
  using message_packet = etl::message_packet<TMessageTypes...>;

  Active_Router(etl::message_router_id_t id) : Base(id) {};

  void receive(const etl::imessage &msg) override {
    if (Base::accepts(msg)) {
      m_queue.emplace(msg);
    }
  }

  void process_queue() override {
    message_packet packet;

    while (m_queue.pop(packet)) {
      etl::imessage &msg = packet.get();
      Base::receive(msg);
    }
  }

  uint32_t get_queue_size() const override { return m_queue.size(); }

  void on_receive_unknown(const etl::imessage &) {
    m_unknown_message_flag = true;
  }

  bool get_unknown_msg_flag() { return m_unknown_message_flag; }

  void reset_unknown_msg_flag() { m_unknown_message_flag = false; }

private:
  etl::queue_spsc_atomic<message_packet, QueueSize> m_queue;
  bool m_unknown_message_flag{};
};