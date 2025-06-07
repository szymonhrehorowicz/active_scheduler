#include "pedal_position_ao.h"
#include "active_object.h"
#include "messages.h"
#include "pedal_position_router.h"
#include <etl/message_bus.h>
#include <iomanip>
#include <iostream>

Pedal_Position_Active_Object::Pedal_Position_Active_Object(
    uint32_t priority, etl::message_router_id_t router_id,
    etl::imessage_bus &public_bus)
    : Active_Object(priority, public_bus) {
  auto *router = new Pedal_Position_Router(router_id);
  assert(router != nullptr);

  router->set_active_object(*this);
  this->set_internal_router(*router);
  m_internal_router = router;
};

void Pedal_Position_Active_Object::update_1ms() {
  std::cout << std::setw(40) << CLASS_NAME << "Updating 1 ms" << std::endl;
  m_position += 1.0f;

  m_public_bus.receive(Pedal_Position_Update_Message{m_position});
}