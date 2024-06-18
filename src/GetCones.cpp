
#include "lap_counter/GetCones.hpp"

using namespace std::chrono_literals;
using namespace std::placeholders;

GetCones::GetCones(std::shared_ptr<DataHolder> data) : Node("cones_reader"), data_(data)
{
    RCLCPP_INFO(this->get_logger(), "Subscribed to %s", CONE_TOPIC_NAME);
    subscriber_ = this->create_subscription<CONE_SUBSCRIBER_TYPE>(
        CONE_TOPIC_NAME, 10, std::bind(&GetCones::topicCallback, this, _1));
}

void GetCones::topicCallback(const CONE_SUBSCRIBER_TYPE::SharedPtr msg) const
{
    // convert cones
    std::unique_ptr<cone_node> header = std::make_unique<cone_node>();
    cone_node *last_cone = header.get();

    for (const auto &cone : msg->cones)
    {
        // Ignore all cones that are not large orange cones
        if (cone.class_type.data != CONE_LARGE_ORANGE_TYPE)
        {
            continue;
        }

        // Convert cone type to data
        position convertedData;
        convertedData.x = cone.position.x;
        convertedData.y = cone.position.y;

        // Create a new node and append it to the linked list
        std::unique_ptr<cone_node> current_node = std::make_unique<cone_node>();
        current_node->next = nullptr;
        current_node->data = convertedData;

        last_cone->next = std::move(current_node);
        last_cone = last_cone->next.get();
    }
    
    data_->setConeListHeader(std::move(header));
}
