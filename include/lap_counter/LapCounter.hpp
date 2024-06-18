#ifndef LAP_COUNTER_HPP_
#define LAP_COUNTER_HPP_

#include <chrono>
#include <memory>

#include "constants.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include "lart_msgs/msg/cone_array.hpp"
#include "lart_msgs/msg/cone.hpp"
#include "lap_counter/DataHolder.hpp"

class LapCounter : public rclcpp::Node
{
public:
    LapCounter(std::shared_ptr<DataHolder> data);

private:
    // atributes
    std::shared_ptr<DataHolder> data_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<LAP_PUBLISHER_TYPE>::SharedPtr publisher_;
    std_msgs::msg::Int32 laps;
    bool lap_lock;

    // methods
    void topicCallback();
};

#endif /* LAP_COUNTER_HPP_ */