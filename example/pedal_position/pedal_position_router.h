#pragma once

#include "active_router.h"
#include "messages.h"

class Pedal_Position_Active_Object;

class Pedal_Position_Router
    : public Active_Router<10, Pedal_Position_Router, Tick_1ms_Message> {
public:
  Pedal_Position_Router(etl::message_router_id_t id,
                        Active_Object_Interface &owner)
      : Active_Router(id, owner) {}

  void on_receive(const Tick_1ms_Message &);
};
