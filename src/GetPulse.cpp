
#include "lap_counter/GetPulse.hpp"

using namespace std::placeholders;

#pragma region 'PUBLIC'

GetPulse::GetPulse(std::shared_ptr<DataHolder> data) : Node(PULSE_SUBSCRIBER_NAME), data_(data)
{
    RCLCPP_INFO(this->get_logger(), "Subscribed to %s", PULSE_TOPIC_NAME);
    subscriber_ = this->create_subscription<PULSE_SUBSCRIBER_TYPE>(
        PULSE_TOPIC_NAME, 10, std::bind(&GetPulse::topicCallback, this, _1));
}

#pragma endregion


#pragma region 'PRIVATE'

void GetPulse::topicCallback(const PULSE_SUBSCRIBER_TYPE::SharedPtr msg) const
{
    u_int8_t average_front_wheels_pulse = (msg->pulse_count_lf + msg->pulse_count_rf)/2;
    float distance_old = data_->getDistance();
    float distance = PULSE_DIFF_TO_DISTANCE_M(average_front_wheels_pulse - last_pulse) + distance_old;
    
    if (distance_old >= distance)
    {
        distance = PULSE_DIFF_TO_DISTANCE_M((average_front_wheels_pulse + PULSES_PER_REVOLUTION - last_pulse)) + distance_old;
    }

    last_pulse = average_front_wheels_pulse;
    
    //RCLCPP_INFO(this->get_logger(), "Distance now: %f", distance);
    data_->setDistance(distance);
}