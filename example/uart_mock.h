#pragma once

typedef struct {
  bool dummy;
} USART_TypeDef;

bool LL_USART_IsActiveFlag_ORE(USART_TypeDef &) { return false; };
bool LL_USART_ClearFlag_ORE(USART_TypeDef &) { return false; };

bool LL_USART_IsActiveFlag_NE(USART_TypeDef &) { return false; };
bool LL_USART_ClearFlag_NE(USART_TypeDef &) { return false; };

bool LL_USART_IsActiveFlag_FE(USART_TypeDef &) { return false; };
bool LL_USART_ClearFlag_FE(USART_TypeDef &) { return false; };

bool LL_USART_IsActiveFlag_PE(USART_TypeDef &) { return false; };
bool LL_USART_ClearFlag_PE(USART_TypeDef &) { return false; };

bool LL_USART_IsActiveFlag_RXNE(USART_TypeDef &) { return false; };
bool LL_USART_ReceiveData8(USART_TypeDef &) { return false };