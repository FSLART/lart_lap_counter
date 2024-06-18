
#include "lap_counter/LapCounter.hpp"

using namespace std::chrono_literals;

LapCounter::LapCounter(std::shared_ptr<DataHolder> data) : Node("count_laps"), data_(data)
{
    laps.data = 0;
    publisher_ = this->create_publisher<std_msgs::msg::Int32>("LapCount", 5);
    timer_ = this->create_wall_timer(PUBLISHER_TIMER, std::bind(&LapCounter::topicCallback, this));
}

void LapCounter::topicCallback()
{

    // see if any cones

    // check if distance is admissible

    // lock lap counter until no cones are seen

    // if passing cones add lap

    // clear cones

    std::unique_ptr<cone_node> current_node = data_->getConeListHeader();
    if (current_node != nullptr)
    {
        current_node = std::move(current_node->next); 
        while (current_node != nullptr)
        {
            RCLCPP_INFO(this->get_logger(), "Positions: %f, %f", current_node->data.x, current_node->data.y);
            current_node = std::move(current_node->next);
        }
        

        
        data_->setConeListHeader(nullptr);
    }

    // RCLCPP_INFO(this->get_logger(), "Publishing lap count %d", laps.data);
    publisher_->publish(laps);
}