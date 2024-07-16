#ifndef GET_PULSE_HPP_
#define GET_PULSE_HPP_

#pragma region 'LIBS'
#include <memory>

#include "lap_counter/constants.hpp"
#include "lart_common/lart_common.h"
#include "rclcpp/rclcpp.hpp"
#include "lart_msgs/msg/dynamics.hpp"
#include "lap_counter/DataHolder.hpp"
#pragma endregion

class GetPulse : public rclcpp::Node
{
public:
    GetPulse(std::shared_ptr<DataHolder> data);

private:
    // atributes
    std::shared_ptr<DataHolder> data_;
    rclcpp::Subscription<PULSE_SUBSCRIBER_TYPE>::SharedPtr subscriber_;

    // methods
    void topicCallback(const PULSE_SUBSCRIBER_TYPE::SharedPtr msg) const;
    mutable u_int8_t last_pulse;
};

#endif /* GET_PULSE_HPP_ */