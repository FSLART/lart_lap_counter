
#include "lap_counter/GetPulse.hpp"

using namespace std::placeholders;


GetPulse::GetPulse(std::shared_ptr<DataHolder> data) : Node(PULSE_SUBSCRIBER_NAME), data_(data)
{
    RCLCPP_INFO(this->get_logger(), "Subscribed to %s", PULSE_TOPIC_NAME);
    subscriber_ = this->create_subscription<PULSE_SUBSCRIBER_TYPE>(
        PULSE_TOPIC_NAME, 10, std::bind(&GetPulse::topicCallback, this, _1));
}


void GetPulse::topicCallback(const PULSE_SUBSCRIBER_TYPE::SharedPtr msg) const
{
    float velocity = std::sqrt(msg->twist.twist.linear.x * msg->twist.twist.linear.x + msg->twist.twist.linear.y * msg->twist.twist.linear.y);
    float time = msg->header.stamp.sec + msg->header.stamp.nanosec * 1e-9; 
    float distance = (velocity + last_velocity) * (time - last_time) / 2 + data_->getDistance();

    last_time = time;
    last_velocity = velocity;

    //RCLCPP_INFO(this->get_logger(), "Distance now: %f", distance);

    data_->setDistance(distance);
}