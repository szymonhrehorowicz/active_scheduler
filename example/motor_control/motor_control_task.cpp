#include "motor_control_task.h"
#include <iomanip>
#include <iostream>

namespace
{

Active::Protection_Data evaluate_angle(float angle, uint32_t aux_code)
{
    Active::Protection_Data data{};

    if (angle > 90.0f)
    {
        data.is_triggered = true;
        data.aux_code = aux_code;
    }

    return data;
}

} // namespace

Motor_Control_Task::Motor_Control_Task(uint32_t priority, etl::message_router_id_t router_id,
                                       etl::imessage_bus &public_bus)
    : AO(priority, router_id, public_bus),
      m_angle_protection(Active::Protection_Manager::instance().get_bus(), Active::Protection_Events::Alarm,
                         [&angle = m_angle]() -> Active::Protection_Data { return evaluate_angle(angle, 0); }),
      m_position_protection(Active::Protection_Manager::instance().get_bus(), Active::Protection_Events::Alarm,
                            [&angle = m_angle]() -> Active::Protection_Data { return evaluate_angle(angle, 1); })
{
}

void Motor_Control_Task::update_1ms()
{
    std::cout << std::setw(40) << CLASS_NAME << "Updating 1 ms" << std::endl;
    for (auto protection : m_protections)
    {
        protection.evaluate();
    }
}

void Motor_Control_Task::update_steering_wheel_data(float angle)
{
    std::cout << std::setw(40) << CLASS_NAME << "New steering wheel angle received: " << angle << std::endl;
    m_angle = angle;
}

void Motor_Control_Task::update_pedal_position_data(float position)
{
    std::cout << std::setw(40) << CLASS_NAME << "New pedal position received: " << position << std::endl;
    m_position = position;
}