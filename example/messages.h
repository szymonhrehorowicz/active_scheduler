#pragma once

#include "active_message.h"

enum MessageId {
  TICK_1MS,
  STEERING_WHEEL_UPDATE,
  PEDAL_POSITION_UPDATE,
  NUMBER_OF_MESSAGES
};

using Tick_1ms_Message = Active::Message<MessageId::TICK_1MS>;

using Steering_Wheel_Update_Message =
    Active::Message<STEERING_WHEEL_UPDATE, float>;

using Pedal_Position_Update_Message =
    Active::Message<PEDAL_POSITION_UPDATE, float>;