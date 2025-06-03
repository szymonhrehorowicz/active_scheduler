#include "interrupt.h"
#include "messages.h"
#include <iostream>

etl::atomic<bool> running{true};

void input_thread(ActiveScheduler &scheduler) {
  while (running) {
    char c = std::cin.get();
    if (c == 'i') {
      scheduler.get_router().receive(IncrementWorkMessage{});
    } else if (c == 'd') {
      scheduler.get_router().receive(DecrementWorkMessage{});
    } else if (c == 'q') {
      running = false;
    }
  }
}

void turn_off_thread() { running = false; }