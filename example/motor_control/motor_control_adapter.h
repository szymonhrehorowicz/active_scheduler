#pragma once

#include "active/active_router.h"
#include "messages.h"

class Motor_Control_Task;
static constexpr uint32_t QUEUE_SIZE_MOTOR_CONTROL{10};

class Motor_Control_Adapter
    : public Active::Router<QUEUE_SIZE_MOTOR_CONTROL, Motor_Control_Adapter,
                            Tick_1ms_Message, Steering_Wheel_Update_Message,
                            Pedal_Position_Update_Message> {

  using Router = Active::Router<QUEUE_SIZE_MOTOR_CONTROL, Motor_Control_Adapter,
                                Tick_1ms_Message, Steering_Wheel_Update_Message,
                                Pedal_Position_Update_Message>;

public:
  Motor_Control_Adapter(etl::message_router_id_t id,
                        Active::Object_Interface &owner)
      : Router(id, owner) {}

  void on_receive(const Tick_1ms_Message &);
  void on_receive(const Steering_Wheel_Update_Message &);
  void on_receive(const Pedal_Position_Update_Message &);

private:
  Motor_Control_Task &get_task();
};
