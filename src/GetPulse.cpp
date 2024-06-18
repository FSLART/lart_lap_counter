
#include "lap_counter/GetPulse.hpp"

using namespace std::chrono_literals;
using namespace std::placeholders;

GetPulse::GetPulse(std::shared_ptr<DataHolder> data) : Node("pulse_reader"), data_(data)
{
    RCLCPP_INFO(this->get_logger(), "Subscribed to %s", PULSE_TOPIC_NAME);
    subscriber_ = this->create_subscription<PULSE_SUBSCRIBER_TYPE>(
        PULSE_TOPIC_NAME, 10, std::bind(&GetPulse::topicCallback, this, _1));
}

void GetPulse::topicCallback(const PULSE_SUBSCRIBER_TYPE::SharedPtr msg) const
{
    u_int8_t average_front_wheels_pulse = (msg->pulse_count_lf + msg->pulse_count_rf)/2;
    float distance_old = data_->getDistance();
    float distance = (average_front_wheels_pulse / PULSES_PER_REVOLUTION) * METERS_PER_REVOLUTION + distance_old;

    if (distance_old >= distance)
    {
        distance = (average_front_wheels_pulse / PULSES_PER_REVOLUTION) * METERS_PER_REVOLUTION + distance_old;
    }
    
    data_->setDistance(distance);
}