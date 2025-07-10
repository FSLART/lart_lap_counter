#ifndef LAP_COUNTER_HPP_
#define LAP_COUNTER_HPP_

#include <chrono>
#include <memory>
#include <cmath>

#include "geometry_msgs/msg/pose_stamped.hpp"
#include "lap_counter/constants.hpp"
#include "lart_common.h"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/u_int16.hpp"
#include "lap_counter/DataHolder.hpp"

class LapCounter : public rclcpp::Node
{
public:
    LapCounter(std::shared_ptr<DataHolder> data);
    void resetLapCount();

private:
    // atributes
    const std::shared_ptr<DataHolder> data_;

    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr ekf_subscriber_;
    geometry_msgs::msg::PoseStamped last_pose_;

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<LAP_PUBLISHER_TYPE>::SharedPtr publisher_;
    std::list<cone_data, std::allocator<cone_data>> cone_list;
    float distance_after_lap;
    float last_callback_distance;
    int16_t laps;

    // methods
    void topicCallback();
    void verifyLap();
    void poseCallback(const geometry_msgs::msg::PoseStamped::SharedPtr msg);
    bool isSameCone(const cone_data &old_cone, const cone_data &new_cone, float new_distance);
};

#endif /* LAP_COUNTER_HPP_ */