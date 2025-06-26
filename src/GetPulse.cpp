
#include "lap_counter/GetPulse.hpp"

using namespace std::placeholders;


GetPulse::GetPulse(std::shared_ptr<DataHolder> data) : Node(PULSE_SUBSCRIBER_NAME), data_(data)
{
    RCLCPP_INFO(this->get_logger(), "Subscribed to %s", PULSE_TOPIC_NAME);
    subscriber_ = this->create_subscription<PULSE_SUBSCRIBER_TYPE>(
        PULSE_TOPIC_NAME, 10, std::bind(&GetPulse::topicCallback, this, _1));
    this->last_velocity = 0.0f;
    this->last_time = 0.0f;
    this->data_->setDistance(0.0f); // Initialize distance to 0
}


void GetPulse::topicCallback(const PULSE_SUBSCRIBER_TYPE::SharedPtr msg) const
{
    u_int16_t rpm = msg->rpm;
    float velocity = RPM_TO_MS(rpm);
    // RCLCPP_INFO(this->get_logger(), "Velocity now: %f", velocity);

    double time = this->now().seconds();
    // RCLCPP_INFO(this->get_logger(), "Time now: %f", time);

    float distance = (velocity + last_velocity) * (time - last_time) / 2 + data_->getDistance();

    last_time = time;
    last_velocity = velocity;

    RCLCPP_INFO(this->get_logger(), "Distance now: %f", distance);

    data_->setDistance(distance);
}