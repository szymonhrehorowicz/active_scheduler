#pragma once

#include "etl/message.h"
#include "etl/message_bus.h"
#include "etl/message_router.h"
#include "etl/task.h"
#include "messages.h"

class ActiveScheduler;

class ActiveTask : public etl::task,
                   public etl::message_router<ActiveTask, IncrementWorkMessage,
                                              DecrementWorkMessage> {
public:
  ActiveTask(int priority, int router_id) : task(priority), m_work(2) {};

  virtual uint32_t task_request_work() const = 0;
  virtual void task_process_work() = 0;

  void on_receive(const IncrementWorkMessage &);
  void on_receive(const DecrementWorkMessage &);
  void on_receive_unknown(const etl::imessage &);

protected:
  int m_work;
};