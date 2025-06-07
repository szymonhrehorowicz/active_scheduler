#include "pedal_position_adapter.h"
#include "pedal_position_task.h"

Pedal_Position_Task &Pedal_Position_Adapter::get_task() {
  return static_cast<Pedal_Position_Task &>(m_owner);
}

void Pedal_Position_Adapter::on_receive(const Tick_1ms_Message &) {
  get_task().update_1ms();
}