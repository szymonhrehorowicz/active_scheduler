#include "pedal_position_router.h"
#include "pedal_position_ao.h"

void Pedal_Position_Router::on_receive(const Tick_1ms_Message &) {
  auto &active_object = static_cast<Pedal_Position_Active_Object &>(m_owner);
  active_object.update_1ms();
}