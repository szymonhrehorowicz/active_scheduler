#include "interrupt.h"
#include "messages.h"
#include <iostream>
#include <string>

etl::atomic<bool> running{true};

void input_thread(etl::imessage_bus &bus, etl::ischeduler &scheduler) {
  std::string input;
  while (running) {
    std::getline(std::cin, input);

    if (input.empty())
      continue;

    char command = input[0];
    float value = 0.0f;

    if (input.size() > 2 && input[1] == ':') {
      try {
        value = std::stof(input.substr(2));
      } catch (...) {
        std::cout << "Invalid value format. Use command:value (e.g., a:80.0)"
                  << std::endl;
        continue;
      }
    }

    switch (command) {
    case 't':
      bus.receive(Tick_1ms_Message{});
      break;

    case 'a': {
      if (input.size() > 2) {
        bus.receive(Steering_Wheel_Update_Message{value});
      } else {
        std::cout << "Missing angle value. Use a:angle (e.g., a:80.0)"
                  << std::endl;
      }
      break;
    }

    case 'p': {
      if (input.size() > 2) {
        bus.receive(Pedal_Position_Update_Message{value});
      } else {
        std::cout << "Missing position value. Use p:position (e.g., p:0.75)"
                  << std::endl;
      }
      break;
    }

    case 'q':
      scheduler.exit_scheduler();
      running = false;
      break;

    case 'h':
    case '?':
      std::cout
          << "Available commands:\n"
          << "  t       - Send tick message\n"
          << "  a:value - Send steering wheel angle update (e.g., a:80.0)\n"
          << "  p:value - Send pedal position update (e.g., p:0.75)\n"
          << "  q       - Quit\n"
          << "  h/?     - Show this help\n";
      break;

    default:
      std::cout << "Unknown command. Type h or ? for help." << std::endl;
      break;
    }
  }
}

void turn_off_thread() { running = false; }