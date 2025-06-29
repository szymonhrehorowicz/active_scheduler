#pragma once

#include "active_message.h"
#include <cstdint>

namespace UART {

// Message IDs for UART communication
enum class MessageId : uint8_t {
  RawDataReceived, // Raw received data
  Error,           // Error conditions
  SetSpeed,        // Set motor speed
  SetPosition,     // Set position
  GetStatus,       // Get device status
  Count
};

// Message type definitions
using SpeedMessage =
    Active::Message<static_cast<etl::message_id_t>(MessageId::SetSpeed), float>;
using PositionMessage =
    Active::Message<static_cast<etl::message_id_t>(MessageId::SetPosition),
                    uint16_t>;
using StatusMessage =
    Active::Message<static_cast<etl::message_id_t>(MessageId::GetStatus)>;

// Message for raw received UART data
class RawDataMessage : public Active::Message<static_cast<etl::message_id_t>(
                                                  MessageId::RawDataReceived),
                                              const uint8_t *, size_t> {
public:
  explicit RawDataMessage(const uint8_t *data, size_t length)
      : Active::Message<static_cast<etl::message_id_t>(
                            MessageId::RawDataReceived),
                        const uint8_t *, size_t>(data, length) {}
};

// Message for UART errors
class ErrorMessage
    : public Active::Message<static_cast<etl::message_id_t>(MessageId::Error),
                             const char *> {
public:
  enum class ErrorType {
    FRAMING_ERROR,
    PARITY_ERROR,
    OVERFLOW_ERROR,
    BUFFER_FULL
  };

  ErrorMessage(ErrorType type, const char *description)
      : Active::Message<static_cast<etl::message_id_t>(MessageId::Error),
                        const char *>(description),
        error_type(type) {}

  const ErrorType error_type;
};

// Abstract base class for message validation
class MessageValidator {
public:
  virtual ~MessageValidator() = default;
  virtual bool validate(const uint8_t *data, size_t length) const = 0;
};

// Factory class for creating messages from raw UART data
class MessageFactory {
public:
  MessageFactory() = default;

  // Register a validator for message verification
  void register_validator(const MessageValidator *validator) {
    validator_ = validator;
  }

  // Create a message from raw UART data
  template <typename T>
  bool try_create(const uint8_t *data, size_t length, T &message) const {
    if (!validator_ || validator_->validate(data, length)) {
      return create_message_impl(data, length, message);
    }
    return false;
  }

private:
  const MessageValidator *validator_ = nullptr;

  // Implementation for specific message types
  template <typename T>
  bool create_message_impl(const uint8_t *data, size_t length,
                           T &message) const;
};

// Template specializations for creating specific message types
template <>
inline bool MessageFactory::create_message_impl(const uint8_t *data,
                                                size_t length,
                                                SpeedMessage &message) const {
  if (length >= sizeof(float) + 1 &&
      data[0] == static_cast<uint8_t>(MessageId::SetSpeed)) {
    float speed;
    memcpy(&speed, data + 1, sizeof(float));
    message = SpeedMessage(speed);
    return true;
  }
  return false;
}

template <>
inline bool
MessageFactory::create_message_impl(const uint8_t *data, size_t length,
                                    PositionMessage &message) const {
  if (length >= sizeof(uint16_t) + 1 &&
      data[0] == static_cast<uint8_t>(MessageId::SetPosition)) {
    uint16_t position;
    memcpy(&position, data + 1, sizeof(uint16_t));
    message = PositionMessage(position);
    return true;
  }
  return false;
}

template <>
inline bool MessageFactory::create_message_impl(const uint8_t *data,
                                                size_t length,
                                                StatusMessage &message) const {
  if (length >= 1 && data[0] == static_cast<uint8_t>(MessageId::GetStatus)) {
    message = StatusMessage();
    return true;
  }
  return false;
}

// Basic message validator that checks message format
class BasicMessageValidator : public MessageValidator {
public:
  bool validate(const uint8_t *data, size_t length) const override {
    if (!data || length < 1)
      return false;

    // Check if the message ID is valid
    MessageId id = static_cast<MessageId>(data[0]);
    if (id >= MessageId::Count)
      return false;

    // Check minimum length based on message type
    switch (id) {
    case MessageId::SetSpeed:
      return length >= sizeof(float) + 1;
    case MessageId::SetPosition:
      return length >= sizeof(uint16_t) + 1;
    case MessageId::GetStatus:
      return length == 1;
    default:
      return false;
    }
  }
};

// CRC-based message validator that checks message integrity
class CrcMessageValidator : public MessageValidator {
public:
  bool validate(const uint8_t *data, size_t length) const override {
    if (!data || length < 2)
      return false; // Need at least message ID and CRC

    // Last byte should be CRC
    uint8_t received_crc = data[length - 1];
    uint8_t calculated_crc = calculate_crc(data, length - 1);

    return received_crc == calculated_crc;
  }

private:
  uint8_t calculate_crc(const uint8_t *data, size_t length) const {
    uint8_t crc = 0xFF;
    for (size_t i = 0; i < length; i++) {
      crc ^= data[i];
      for (int j = 0; j < 8; j++) {
        if (crc & 0x80) {
          crc = (crc << 1) ^ 0x31; // CRC-8 polynomial
        } else {
          crc <<= 1;
        }
      }
    }
    return crc;
  }
};

} // namespace UART
