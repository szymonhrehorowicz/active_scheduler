#pragma once

#include "active_object.h"
#include "pedal_position_router.h"
#include <string_view>

class Pedal_Position_Active_Object
    : public Active_Object<Pedal_Position_Router> {
public:
  static constexpr std::string_view CLASS_NAME{
      "Pedal_Position_Active_Object | "};

  Pedal_Position_Active_Object(uint32_t priority,
                               etl::message_router_id_t router_id,
                               etl::imessage_bus &public_bus)
      : Active_Object(priority, router_id, public_bus) {};

  void update_1ms();

private:
  float m_position{0.0f};
};