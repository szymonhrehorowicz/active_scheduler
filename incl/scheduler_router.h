#pragma once

#include "etl/message_bus.h"
#include "etl/message_router.h"
#include "etl/queue_spsc_atomic.h"
#include "messages.h"

constexpr size_t NUM_OF_TASKS{10};

class SchedulerRouter
    : public etl::message_router<SchedulerRouter, IncrementWorkMessage,
                                 DecrementWorkMessage> {
public:
  using Base = etl::message_router<SchedulerRouter, IncrementWorkMessage,
                                   DecrementWorkMessage>;
  SchedulerRouter(int router_id) : message_router(router_id) {};

  void receive(const etl::imessage &msg) override;
  void process_queue();

  void on_receive(const IncrementWorkMessage &msg);
  void on_receive(const DecrementWorkMessage &msg);
  void on_receive_unknown(const etl::imessage &msg);

  etl::imessage_bus &get_internal_bus();

private:
  etl::queue_spsc_atomic<message_packet, 10> m_queue;
  etl::message_bus<NUM_OF_TASKS> m_internal_bus;
};
