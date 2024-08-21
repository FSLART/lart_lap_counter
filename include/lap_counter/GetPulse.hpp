#ifndef GET_PULSE_HPP_
#define GET_PULSE_HPP_

#include <memory>

#include "lap_counter/constants.hpp"
#include "lart_common/lart_common.h"
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "lap_counter/DataHolder.hpp"

class GetPulse : public rclcpp::Node
{
public:
    GetPulse(std::shared_ptr<DataHolder> data);

private:
    // atributes
    std::shared_ptr<DataHolder> data_;
    rclcpp::Subscription<PULSE_SUBSCRIBER_TYPE>::SharedPtr subscriber_;
    mutable float last_velocity;
    mutable float last_time;

    // methods
    void topicCallback(const PULSE_SUBSCRIBER_TYPE::SharedPtr msg) const;
};

#endif /* GET_PULSE_HPP_ */