
#include "lap_counter/GetMission.hpp"

using namespace std::placeholders;

#pragma region 'PUBLIC'

GetMission::GetMission(std::shared_ptr<DataHolder> data) : Node(MISSION_SUBSCRIBER_NAME), data_(data)
{
    RCLCPP_INFO(this->get_logger(), "Subscribed to %s", PULSE_TOPIC_NAME);
    subscriber_ = this->create_subscription<MISSION_SUBSCRIBER_TYPE>(
        MISSION_TOPIC_NAME, 10, std::bind(&GetMission::topicCallback, this, _1));
}

#pragma endregion


#pragma region 'PRIVATE'

void GetMission::topicCallback(const MISSION_SUBSCRIBER_TYPE::SharedPtr msg) const{
    
    data_->setMissionType(msg->data);
}

#pragma endregion

