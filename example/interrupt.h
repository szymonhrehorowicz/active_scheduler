#pragma once

#include "etl/message_bus.h"

void input_thread(etl::imessage_bus &task);
void turn_off_thread();