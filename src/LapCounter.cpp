
#include "lap_counter/LapCounter.hpp"

using namespace std::chrono_literals;

#pragma region 'PUBLIC'

LapCounter::LapCounter(std::shared_ptr<DataHolder> data)
    : Node("count_laps"), data_(data), last_distance_known{0.0, 0.0, 0.0, 0.0}
{
    // ros args
    this->declare_parameter("lap_min", rclcpp::ParameterValue(DEFAULT_DISTANCES[0]));
    this->declare_parameter("lap_max", rclcpp::ParameterValue(DEFAULT_DISTANCES[1]));
    this->declare_parameter("lap_track_width", rclcpp::ParameterValue(TRACK_WIDTH));

    laps.data = 0;
    publisher_ = this->create_publisher<LAP_PUBLISHER_TYPE>(LAP_PUBLISHER_NAME, 5);
    timer_ = this->create_wall_timer(PUBLISHER_TIMER, std::bind(&LapCounter::topicCallback, this));
}

#pragma endregion

#pragma region 'PRIVATE'

bool compare_cone(const cone_data &first, const cone_data &second)
{
    return std::sqrt(first.pos.x * first.pos.x + first.pos.y * first.pos.y) > std::sqrt(second.pos.x * second.pos.x + second.pos.y * second.pos.y);
}

void LapCounter::topicCallback()
{
    std::list<cone_data> cone_list_data = data_->getConeList();
    if (cone_list_data.empty())
    {
        if (distance_after_lap > -1)
        {
            float delta_distance = data_->getDistance() - distance_after_lap;
            if (delta_distance > SAFE_LAP_DISTANCE)
            {
                distance_after_lap = -1;
            }
            else
            {
                distance_after_lap += delta_distance;
            }
        }

        goto callback_end;
    }

    switch (data_->getMissionType())
    {
        case  lart_msgs::msg::Mission::ACCELERATION :
            this->set_parameter(rclcpp::Parameter("lap_min", MISSION_1_DISTANCE[0]));
            this->set_parameter(rclcpp::Parameter("lap_min", MISSION_1_DISTANCE[1]));
            break;
        case  lart_msgs::msg::Mission::SKIDPAD :
            this->set_parameter(rclcpp::Parameter("lap_min", MISSION_2_DISTANCE[0]));
            this->set_parameter(rclcpp::Parameter("lap_min", MISSION_2_DISTANCE[1]));
            break;
        case  lart_msgs::msg::Mission::TRACKDRIVE :
            this->set_parameter(rclcpp::Parameter("lap_min", MISSION_3_DISTANCE[0]));
            this->set_parameter(rclcpp::Parameter("lap_min", MISSION_3_DISTANCE[1]));
    }

    if (data_->getDistance() < this->get_parameter("lap_min").as_double() * (laps.data + 1) || data_->getDistance() > this->get_parameter("lap_max").as_double() * (laps.data + 1))
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
        goto callback_end;
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

        bool positive = false, negative = false;
        short in_range = 0;
        float track_width = (float)this->get_parameter("lap_track_width").as_double();
        for (auto &cone : this->cone_list)
        {
            if (std::sqrt(cone.pos.x * cone.pos.x + cone.pos.y * cone.pos.y) < track_width)
            {
                in_range++;
                if (cone.pos.x < 0)
                {
                    negative = true;
                }
                else
                {
                    positive = true;
                }
            }
        }

        if (positive && negative && in_range > 1 && distance_after_lap == -1)
        {
            distance_after_lap = data_->getDistance();
            laps.data++;
        }
    }

    cone_list_data.clear();
    data_->setConeList(cone_list_data);

callback_end:
    publisher_->publish(laps);
}

bool LapCounter::isSameCone(const cone_data &old_cone, const cone_data &new_cone, short index)
{

    float deltaConeDistance = std::sqrt(std::pow(new_cone.pos.x - old_cone.pos.x, 2) + std::pow(new_cone.pos.y - old_cone.pos.y, 2));

    return std::fabs(deltaConeDistance - this->last_distance_known[index]) < ACCEPTABLE_CONE_DISTANCE_ERROR;
}

#pragma endregion