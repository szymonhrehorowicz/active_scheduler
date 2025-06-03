#pragma once

#include "etl/message.h"

enum MessageId { INCREMENT_WORK, DECREMENT_WORK, NUMBER_OF_MESSAGES };

using IncrementWorkMessage = etl::message<MessageId::INCREMENT_WORK>;
using DecrementWorkMessage = etl::message<MessageId::DECREMENT_WORK>;