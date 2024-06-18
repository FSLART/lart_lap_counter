#ifndef GET_PULSE_HPP_
#define GET_PULSE_HPP_

#include <chrono>
#include <memory>

#include "constants.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include "lart_msgs/msg/dynamics.hpp"
#include "lap_counter/DataHolder.hpp"

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
};

#endif /* GET_PULSE_HPP_ */