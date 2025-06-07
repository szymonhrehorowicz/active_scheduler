#include "pedal_position_task.h"
#include <iomanip>
#include <iostream>

void Pedal_Position_Task::update_1ms() {
  std::cout << std::setw(40) << CLASS_NAME << "Updating 1 ms" << std::endl;
  m_position += 1.0f;

  m_public_bus.receive(Pedal_Position_Update_Message{m_position});
}