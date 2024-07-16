
#include "lap_counter/GetCones.hpp"

using namespace std::chrono_literals;
using namespace std::placeholders;

#pragma region 'PUBLIC'

GetCones::GetCones(std::shared_ptr<DataHolder> data) : Node(CONE_SUBSCRIBER_NAME), data_(data)
{
    RCLCPP_INFO(this->get_logger(), "Subscribed to %s", CONE_TOPIC_NAME);
    subscriber_ = this->create_subscription<CONE_SUBSCRIBER_TYPE>(
        CONE_TOPIC_NAME, 10, std::bind(&GetCones::topicCallback, this, _1));
}

#pragma endregion


#pragma region 'PRIVATE'

void GetCones::topicCallback(const CONE_SUBSCRIBER_TYPE::SharedPtr msg) const
{
    std::list<cone_data> list;

    for (const auto &msg_cone : msg->cones)
    {
        //RCLCPP_INFO(this->get_logger(), "Cone: %d", msg_cone.class_type.data);
        if (msg_cone.class_type.data != CONE_LARGE_ORANGE_TYPE)
        {
            continue;
        }

        // Convert message data
        cone_data convertedData;
        convertedData.pos.x  = msg_cone.position.x;
        convertedData.pos.y = msg_cone.position.y;


        list.emplace_back(convertedData);
    }
    
    data_->setConeList(std::move(list));
}

#pragma endregion