#pragma once

#include "active/active_object.h"
#include "active/active_protection.h"
#include "motor_control_adapter.h"
#include <string_view>

class Motor_Control_Task : public Active::Object<Motor_Control_Adapter>
{
    using AO = Active::Object<Motor_Control_Adapter>;

  public:
    static constexpr std::string_view CLASS_NAME{"Motor_Control_Task | "};

    Motor_Control_Task(uint32_t priority, etl::message_router_id_t router_id, etl::imessage_bus &public_bus);

    void update_1ms();
    void update_steering_wheel_data(float angle);
    void update_pedal_position_data(float position);

  private:
    float m_angle{};
    float m_position{};
    Active::Protection<69> m_angle_protection;
    Active::Protection<69> m_position_protection;
    etl::vector<Active::Protection_Object, 2> m_protections{m_angle_protection, m_position_protection};
};