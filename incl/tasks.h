#pragma once

#include "etl/message.h"
#include "etl/message_bus.h"
#include "etl/message_router.h"
#include "etl/queue.h"
#include "etl/scheduler.h"
#include "etl/task.h"
#include "messages.h"
#include <iostream>

constexpr size_t NUM_OF_TASKS{10};

class SchedulerRouter
    : public etl::message_router<SchedulerRouter, IncrementWorkMessage,
                                 DecrementWorkMessage> {
public:
  using Base = etl::message_router<SchedulerRouter, IncrementWorkMessage,
                                   DecrementWorkMessage>;
  SchedulerRouter(int router_id) : message_router(router_id) {};

  void receive(const etl::imessage &msg_) override {
    if (accepts(msg_)) {
      m_queue.emplace(msg_);

      std::cout << "Queueing message " << int(msg_.get_message_id())
                << std::endl;
    } else {
      std::cout << "Ignoring message " << int(msg_.get_message_id())
                << std::endl;
    }
  }

  void process_queue() {
    while (!m_queue.empty()) {
      message_packet &packet = m_queue.front();
      etl::imessage &msg = packet.get();
      std::cout << "Processing message " << int(msg.get_message_id())
                << std::endl;
      Base::receive(msg);
      m_queue.pop();
    }
  }

  void on_receive(const IncrementWorkMessage &msg) {
    std::cout << "Router sending increment work message" << std::endl;
    m_internal_bus.receive(msg);
  }

  void on_receive(const DecrementWorkMessage &msg) {
    std::cout << "Router sending decrement work message" << std::endl;
    m_internal_bus.receive(msg);
  }

  void on_receive_unknown(const etl::imessage &msg) {
    std::cout << "  Received unknown message " << int(msg.get_message_id())
              << std::endl;
  }

  etl::imessage_bus &get_internal_bus() { return m_internal_bus; }

private:
  etl::queue<message_packet, 10> m_queue;
  etl::message_bus<NUM_OF_TASKS> m_internal_bus;
};

class ActiveTask : public etl::task,
                   public etl::message_router<ActiveTask, IncrementWorkMessage,
                                              DecrementWorkMessage> {
public:
  ActiveTask(int priority, int router_id) : task(priority), m_work(2) {};

  void on_receive(const IncrementWorkMessage &);
  void on_receive(const DecrementWorkMessage &);
  void on_receive_unknown(const etl::imessage &);

protected:
  int m_work;
};

class ActiveScheduler
    : public etl::scheduler<etl::scheduler_policy_highest_priority,
                            NUM_OF_TASKS> {
public:
  using Base =
      etl::scheduler<etl::scheduler_policy_highest_priority, NUM_OF_TASKS>;

  SchedulerRouter &get_router() { return m_router; }

  void add_task(ActiveTask &task) {
    Base::add_task(task);
    m_router.get_internal_bus().subscribe(task);
  }

private:
  SchedulerRouter m_router{0};
};

class IdleHandler {
public:
  IdleHandler(etl::ischeduler &scheduler) : m_scheduler(scheduler) {};

  void idle_callback();

private:
  etl::ischeduler &m_scheduler;
};