
#include "lap_counter/GetMission.hpp"

using namespace std::placeholders;

GetMission::GetMission(std::shared_ptr<DataHolder> data, std::shared_ptr<LapCounter> publisher_node_) : Node(MISSION_SUBSCRIBER_NAME), data_(data), lap_counter_node(publisher_node_)
{
    last_mission = lart_msgs::msg::Mission::INSPECTION;
    RCLCPP_INFO(this->get_logger(), "Subscribed to %s", MISSION_TOPIC_NAME);
    subscriber_ = this->create_subscription<MISSION_SUBSCRIBER_TYPE>(
        MISSION_TOPIC_NAME, 10, std::bind(&GetMission::topicCallback, this, _1));
}

void GetMission::topicCallback(const MISSION_SUBSCRIBER_TYPE::SharedPtr msg) const
{
    if (msg->mission.data != last_mission)
    {
        last_mission == msg->mission.data;
        switch (msg->mission.data)
        {
        case lart_msgs::msg::Mission::ACCELERATION:
            lap_counter_node->set_parameter(rclcpp::Parameter("lap_min", MISSION_1_DISTANCE_MIN));
            lap_counter_node->set_parameter(rclcpp::Parameter("lap_min", MISSION_1_DISTANCE_MAX));
            break;
        case lart_msgs::msg::Mission::SKIDPAD:
            lap_counter_node->set_parameter(rclcpp::Parameter("lap_min", MISSION_2_DISTANCE_MIN));
            lap_counter_node->set_parameter(rclcpp::Parameter("lap_min", MISSION_2_DISTANCE_MAX));
            break;
        case lart_msgs::msg::Mission::TRACKDRIVE:
            lap_counter_node->set_parameter(rclcpp::Parameter("lap_min", MISSION_3_DISTANCE_MIN));
            lap_counter_node->set_parameter(rclcpp::Parameter("lap_min", MISSION_3_DISTANCE_MAX));
        }
        lap_counter_node->resetLapCount();
    }
}
