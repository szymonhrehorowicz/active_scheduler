#pragma once

#include "etl/message_router.h"
#include "etl/queue_spsc_atomic.h"

class Active_Router_Interface : public etl::imessage_router {
public:
  Active_Router_Interface(etl::message_router_id_t id) : imessage_router(id) {}
  virtual void process_queue() = 0;
  virtual uint32_t get_queue_size() const = 0;
  virtual bool get_unknown_msg_flag() const = 0;
  virtual void reset_unknown_msg_flag() = 0;
};

template <uint32_t QueueSize, typename TDerived, typename... TMessageTypes>
class Active_Router : public Active_Router_Interface {
private:
  class Router_Impl
      : public etl::message_router<Router_Impl, TMessageTypes...> {
  public:
    using Base = etl::message_router<Router_Impl, TMessageTypes...>;

    Router_Impl(TDerived &parent, etl::message_router_id_t id)
        : Base(id), m_parent(parent) {}

    template <typename TMessage> void on_receive(const TMessage &msg) {
      m_parent.on_receive(msg);
    }

    void on_receive_unknown(const etl::imessage &msg) {
      m_parent.on_receive_unknown(msg);
    }

  private:
    TDerived &m_parent;
  };

public:
  using message_packet = typename Router_Impl::message_packet;

  Active_Router(etl::message_router_id_t id)
      : Active_Router_Interface(id),
        m_router(*static_cast<TDerived *>(this), id) {}

  void receive(const etl::imessage &msg) override {
    if (m_router.accepts(msg)) {
      m_queue.emplace(msg);
    }
  }

  void process_queue() override {
    message_packet packet;
    while (m_queue.pop(packet)) {
      etl::imessage &msg = packet.get();
      m_router.receive(msg);
    }
  }

  uint32_t get_queue_size() const override { return m_queue.size(); }

  bool accepts(etl::message_id_t id) const override {
    return m_router.accepts(id);
  }

  bool is_null_router() const override { return false; }
  bool is_producer() const override { return true; }
  bool is_consumer() const override { return true; }

  bool get_unknown_msg_flag() const override { return m_unknown_message_flag; }
  void reset_unknown_msg_flag() override { m_unknown_message_flag = false; }

protected:
  virtual void on_receive_unknown(const etl::imessage &) {
    m_unknown_message_flag = true;
  }

private:
  Router_Impl m_router;
  etl::queue_spsc_atomic<message_packet, QueueSize> m_queue;
  bool m_unknown_message_flag{};
};