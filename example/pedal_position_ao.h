#pragma once

#include "active_object.h"
#include <string>

class Pedal_Position_Active_Object : public Active_Object {
public:
  const std::string CLASS_NAME{"Pedal_Position_Active_Object | "};

  Pedal_Position_Active_Object(uint32_t priority,
                               etl::message_router_id_t router_id,
                               etl::imessage_bus &public_bus);

  void update_1ms();

private:
  float m_position{0.0f};
};