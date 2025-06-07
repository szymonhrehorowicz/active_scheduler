#include "pedal_position_router.h"
#include "pedal_position_ao.h"

void Pedal_Position_Router::set_active_object(
    Pedal_Position_Active_Object &obj) {
  assert(m_active_object == nullptr);
  m_active_object = &obj;
}

void Pedal_Position_Router::on_receive(const Tick_1ms_Message &) {
  assert(m_active_object != nullptr);
  m_active_object->update_1ms();
}