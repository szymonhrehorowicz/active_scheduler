#pragma once

#include "active_router.h"
#include "messages.h"

class Pedal_Position_Active_Object;

class Pedal_Position_Router
    : public Active_Router<10, Pedal_Position_Router, Tick_1ms_Message> {
public:
  Pedal_Position_Router(etl::message_router_id_t id)
      : Active_Router(id), m_active_object(nullptr) {}

  void set_active_object(Pedal_Position_Active_Object &obj);

  void on_receive(const Tick_1ms_Message &);

private:
  Pedal_Position_Active_Object *m_active_object;
};
