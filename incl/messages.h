#pragma once

#include "etl/message.h"

enum MessageId {
  TICK_1MS,
  STEERING_WHEEL_UPDATE,
  PEDAL_POSITION_UPDATE,
  NUMBER_OF_MESSAGES
};

using Tick_1ms_Message = etl::message<MessageId::TICK_1MS>;

struct Steering_Wheel_Update_Message
    : public etl::message<STEERING_WHEEL_UPDATE> {
  float angle;
};

struct Pedal_Position_Update_Message
    : public etl::message<PEDAL_POSITION_UPDATE> {
  float position;
};