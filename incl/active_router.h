#pragma once

#include "etl/message_router.h"
#include "etl/queue_spsc_atomic.h"
#include <cstdint>

struct Router_Stats {
  volatile uint32_t overflow_counter{0};
  volatile uint32_t unknown_message_counter{0};
};

class Active_Object_Interface;

class Active_Router_Interface : public etl::imessage_router {
public:
  Active_Router_Interface(etl::message_router_id_t id,
                          Active_Object_Interface &owner)
      : imessage_router(id), m_owner(owner) {}

  virtual void process_queue() = 0;
  virtual uint32_t get_queue_size() const = 0;

protected:
  Active_Object_Interface &m_owner;
};

template <uint32_t QueueSize, typename TDerived, typename... TMessageTypes>
class Active_Router : public Active_Router_Interface {
  static_assert(QueueSize > 0 && QueueSize <= 256,
                "Queue size must be reasonable for embedded system");
  static_assert(sizeof...(TMessageTypes) <= 16,
                "Too many message types may impact stack usage");

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

  Active_Router(etl::message_router_id_t id, Active_Object_Interface &owner)
      : Active_Router_Interface(id, owner),
        m_router(*static_cast<TDerived *>(this), id) {}

  void receive(const etl::imessage &msg) override {
    if (m_router.accepts(msg)) {
      if (!m_queue.emplace(msg)) {
        m_router_stats.overflow_counter += 1;
      }
    }
  }

  void process_queue() override {
    message_packet packet;
    while (m_queue.pop(packet)) {
      m_router.receive(packet.get());
    }
  }

  uint32_t get_queue_size() const override { return m_queue.size(); }

  bool accepts(etl::message_id_t id) const override {
    return m_router.accepts(id);
  }

  bool is_null_router() const override { return false; }
  bool is_producer() const override { return true; }
  bool is_consumer() const override { return true; }

protected:
  virtual void on_receive_unknown(const etl::imessage &) {
    m_router_stats.unknown_message_counter += 1;
  }

private:
  Router_Impl m_router;
  etl::queue_spsc_atomic<message_packet, QueueSize> m_queue;
  Router_Stats m_router_stats;
};