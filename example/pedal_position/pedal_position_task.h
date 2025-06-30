#pragma once

#include "active/active_object.h"
#include "pedal_position_adapter.h"
#include <string_view>

class Pedal_Position_Task : public Active::Object<Pedal_Position_Adapter> {

  using AO = Active::Object<Pedal_Position_Adapter>;

public:
  static constexpr std::string_view CLASS_NAME{"Pedal_Position_Task | "};

  Pedal_Position_Task(uint32_t priority, etl::message_router_id_t router_id,
                      etl::imessage_bus &public_bus)
      : AO(priority, router_id, public_bus) {};

  void update_1ms();

private:
  float m_position{0.0f};
};