#pragma once

#include "active_router.h"
#include "messages.h"

class Pedal_Position_Task;

static constexpr uint32_t QUEUE_SIZE_PEDAL_POSITION{10};

class Pedal_Position_Adapter
    : public Active::Router<QUEUE_SIZE_PEDAL_POSITION, Pedal_Position_Adapter,
                            Tick_1ms_Message> {

  using Router = Active::Router<QUEUE_SIZE_PEDAL_POSITION,
                                Pedal_Position_Adapter, Tick_1ms_Message>;

public:
  Pedal_Position_Adapter(etl::message_router_id_t id,
                         Active::Object_Interface &owner)
      : Router(id, owner) {}

  void on_receive(const Tick_1ms_Message &);

private:
  Pedal_Position_Task &get_task();
};
