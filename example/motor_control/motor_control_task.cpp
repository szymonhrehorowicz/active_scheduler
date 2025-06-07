#include "motor_control_task.h"
#include <iomanip>
#include <iostream>

void Motor_Control_Task::update_1ms() {
  std::cout << std::setw(40) << CLASS_NAME << "Updating 1 ms" << std::endl;
}

void Motor_Control_Task::update_steering_wheel_data(float angle) {
  std::cout << std::setw(40) << CLASS_NAME
            << "New steering wheel angle received: " << angle << std::endl;
}

void Motor_Control_Task::update_pedal_position_data(float position) {
  std::cout << std::setw(40) << CLASS_NAME
            << "New pedal position received: " << position << std::endl;
}