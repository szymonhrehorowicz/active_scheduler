#pragma once

#include "etl/array.h"
#include "etl/circular_buffer.h"
#include "etl/message_router.h"
#include "uart_messages.h"
#include "uart_mock.h"
#include <cstdint>

namespace UART {

class Handler {
public:
  static constexpr size_t MAX_BUFFER_SIZE{64};
  static constexpr uint8_t MESSAGE_END_MARKER = '\n';

  Handler(USART_TypeDef &uart_handle, etl::imessage_router &router)
      : m_uart_handle(uart_handle), m_router(router) {}

  void interrupt_handler(size_t) {
    check_errors();

    bool has_received_message_end = false;
    while (LL_USART_IsActiveFlag_RXNE(m_uart_handle)) {
      uint8_t const data = LL_USART_ReceiveData8(m_uart_handle);
      m_rx_buffer.push(data);
      if (data == MESSAGE_END_MARKER) {
        has_received_message_end = true;
      }
    }

    if (has_received_message_end) {
      process_messages();
    }
  }

private:
  union ErrorFlags {
    struct {
      bool overrun : 1;
      bool noise : 1;
      bool framing : 1;
      bool parity : 1;
      uint8_t unused : 4;
    } bits;
    uint8_t all;
  };

  void process_messages() {
    if (m_rx_buffer.size() < 2) {
      m_rx_buffer.clear();
      return;
    }

    uint8_t id = m_rx_buffer.front();
    m_rx_buffer.pop();

    size_t data_length = 0;
    etl::array<uint8_t, MAX_BUFFER_SIZE> data_buffer;

    while (!m_rx_buffer.empty() && m_rx_buffer.front() != MESSAGE_END_MARKER) {
      if (data_length < data_buffer.size()) {
        data_buffer[data_length++] = m_rx_buffer.front();
      }
      m_rx_buffer.pop();
    }

    if (!m_rx_buffer.empty()) {
      m_rx_buffer.pop();
    }

    etl::message_packet packet;
    if (m_factory.try_create(id, data_buffer.data(), data_length, packet)) {
      m_router.receive(packet);
    }
  }

  void check_errors() {
    if (LL_USART_IsActiveFlag_ORE(m_uart_handle)) {
      m_error_flags.bits.overrun = true;
      LL_USART_ClearFlag_ORE(m_uart_handle);
      // TODO: Send error message
    }
    if (LL_USART_IsActiveFlag_NE(m_uart_handle)) {
      m_error_flags.bits.noise = true;
      LL_USART_ClearFlag_NE(m_uart_handle);
      // TODO: Send error message
    }
    if (LL_USART_IsActiveFlag_FE(m_uart_handle)) {
      m_error_flags.bits.framing = true;
      LL_USART_ClearFlag_FE(m_uart_handle);
      // TODO: Send error message
    }
    if (LL_USART_IsActiveFlag_PE(m_uart_handle)) {
      m_error_flags.bits.parity = true;
      LL_USART_ClearFlag_PE(m_uart_handle);
      // TODO: Send error message
    }
  };

  USART_TypeDef &m_uart_handle;
  etl::imessage_router &m_router;
  etl::circular_buffer<uint8_t, MAX_BUFFER_SIZE> m_rx_buffer{};
  volatile ErrorFlags m_error_flags{};
  MessageFactory m_factory;
};

} // namespace UART