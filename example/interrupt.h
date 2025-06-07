#pragma once

#include "etl/message_bus.h"
#include "etl/scheduler.h"

void input_thread(etl::imessage_bus &bus, etl::ischeduler &scheduler);
void turn_off_thread();