#include "interrupt.h"
#include "messages.h"
#include <iostream>

etl::atomic<bool> running{true};

void input_thread(etl::imessage_bus &bus) {
  while (running) {
    char c = std::cin.get();
    if (c == 'i') {
      bus.receive(IncrementWorkMessage{});
    } else if (c == 'd') {
      bus.receive(DecrementWorkMessage{});
    } else if (c == 'q') {
      running = false;
    }
  }
}

void turn_off_thread() { running = false; }