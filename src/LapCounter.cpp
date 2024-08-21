
#include "lap_counter/LapCounter.hpp"

using namespace std::chrono_literals;


LapCounter::LapCounter(std::shared_ptr<DataHolder> data)
    : Node(LAP_PUBLISHER_NAME), data_(data), last_distance_known{0.0, 0.0, 0.0, 0.0}, distance_after_lap(-1)
{
    // ros args
    this->declare_parameter("lap_min", rclcpp::ParameterValue((float)DEFAULT_DISTANCES_MIN));
    this->declare_parameter("lap_max", rclcpp::ParameterValue((float)DEFAULT_DISTANCES_MAX));
    this->declare_parameter("lap_track_width", rclcpp::ParameterValue((float)TRACK_WIDTH));

    laps = -1;
    publisher_ = this->create_publisher<LAP_PUBLISHER_TYPE>(LAP_PUBLISHER_TOPIC, 5);
    timer_ = this->create_wall_timer(PUBLISHER_TIMER, std::bind(&LapCounter::topicCallback, this));
}



bool compare_cone(const cone_data &first, const cone_data &second)
{
    return std::sqrt(first.pos.x * first.pos.x + first.pos.y * first.pos.y) > std::sqrt(second.pos.x * second.pos.x + second.pos.y * second.pos.y);
}

void LapCounter::topicCallback()
{

    std::list<cone_data> cone_list_data = data_->getConeList();
    if (distance_after_lap > -1)
    {
        float delta_distance = data_->getDistance() - distance_after_lap;
        if (delta_distance > SAFE_LAP_DISTANCE)
        {
            distance_after_lap = -1;
        }
        else
        {
            goto callback_end;
        }
    }

    // Empty list
    if (cone_list_data.empty())
    {
        goto callback_end;
    }

    // Verify distance
    if (data_->getDistance() < this->get_parameter("lap_min").as_double() * (laps + 1) || data_->getDistance() > this->get_parameter("lap_max").as_double() * (laps + 1))
    {
        goto callback_end;
    }

    if (this->cone_list.empty())
    {
        cone_list_data.sort(compare_cone);
        auto iterator = cone_list_data.begin();
        for (short i = 0; i < 4 || iterator != cone_list_data.end(); i++, iterator++)
        {
            this->cone_list.push_back(*iterator);
        }
    }
    else
    {
        for (const auto &node : cone_list)
        {
            short i = 0;
            for (auto iterator = cone_list_data.begin(); iterator != cone_list_data.end(); iterator++, i++)
            {
                if (isSameCone(*iterator, node, i))
                {
                    *iterator = node;
                    break;
                }
            }
        }
    }

    callback_end:

        bool positive = false, negative = false;
        short in_range = 0;
        float track_width = this->get_parameter("lap_track_width").as_double();
        for (auto &cone : this->cone_list)
        {
            if ((float) std::sqrt(cone.pos.x * cone.pos.x + cone.pos.y * cone.pos.y) <= track_width)
            {
                negative = true;
            }
            else
            {
                positive = true;
            }
        }
        //RCLCPP_INFO(this->get_logger(), "in range: %d", in_range);

    if (positive && negative && in_range > 1 && distance_after_lap == -1) 
    {
        distance_after_lap = data_->getDistance();
        cone_list_data.clear();
        data_->setConeList(cone_list_data);
        laps++;
    }

    auto laps = std_msgs::msg::UInt16();
    
    laps.data = this->laps == -1 ? 0 : this->laps;
    
    //RCLCPP_INFO(this->get_logger(), "laps: %d", this->laps);
    //RCLCPP_INFO(this->get_logger(), "distance: %f", data_->getDistance());

    publisher_->publish(laps);
}

bool LapCounter::isSameCone(const cone_data &old_cone, const cone_data &new_cone, short index)
{

    float deltaConeDistance = std::sqrt(std::pow(new_cone.pos.x - old_cone.pos.x, 2) + std::pow(new_cone.pos.y - old_cone.pos.y, 2));

    return std::fabs(deltaConeDistance - this->last_distance_known[index]) < ACCEPTABLE_CONE_DISTANCE_ERROR;
}

void LapCounter::resetLapCount(){
    this->laps = -1;
    data_->setDistance(0);
}