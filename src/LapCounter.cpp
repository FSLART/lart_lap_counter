
#include "lap_counter/LapCounter.hpp"

using namespace std::chrono_literals;

bool compare_cone(const cone_data &first, const cone_data &second)
{
    return sqrt(first.pos.x * first.pos.x + first.pos.y * first.pos.y) > sqrt(second.pos.x * second.pos.x + second.pos.y * second.pos.y);
}

LapCounter::LapCounter(std::shared_ptr<DataHolder> data)
    : Node("count_laps"), data_(data), distance_after_lap(-1), last_callback_distance(0.0), laps(-1)
{
    // ros args
    this->declare_parameter("lap_min", rclcpp::ParameterValue((float)DEFAULT_DISTANCES_MIN));
    this->declare_parameter("lap_max", rclcpp::ParameterValue((float)DEFAULT_DISTANCES_MAX));
    this->declare_parameter("lap_track_width", rclcpp::ParameterValue((float)TRACK_WIDTH));

    publisher_ = this->create_publisher<LAP_PUBLISHER_TYPE>(LAP_PUBLISHER_NAME, 5);
    timer_ = this->create_wall_timer(PUBLISHER_TIMER, std::bind(&LapCounter::topicCallback, this));
}

void LapCounter::topicCallback()
{
    verifyLap();

    auto laps = std_msgs::msg::UInt16();
    laps.data = this->laps == -1 ? 0 : this->laps;

    //RCLCPP_INFO(this->get_logger(), "laps: %d", this->laps);

    publisher_->publish(laps);
}

void LapCounter::verifyLap()
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
            return;
        }
    }

    // Empty list and
    // Verify distance
    if (cone_list_data.empty() || data_->getDistance() < this->get_parameter("lap_min").as_double() * (laps + 1) - 5 || data_->getDistance() > this->get_parameter("lap_max").as_double() * (laps + 1) + 5)
    {
        return;
    }

    if (this->cone_list.empty() || this->cone_list.size() < 4)
    {
        cone_list_data.sort(compare_cone);
        int i = -1;
        for (const auto &cone : cone_list_data)
        {
            i++;
            if (i < (int) this->cone_list.size())
                continue;
            if (i > 3)
                break;
            this->cone_list.push_front(cone);
        }
    }
    else
    {
        float distance_now = data_->getDistance();
        for (auto &old_cone : this->cone_list)
        {
            for (const auto &new_cone : cone_list_data)
            {
                if (isSameCone(old_cone, new_cone, distance_now))
                {
                    old_cone = new_cone;
                    break;
                }
            }
        }
        last_callback_distance = distance_now;

        while (this->cone_list.size() > 4)
        {
            cone_list.pop_back();
        }
    }

    bool positive = false, negative = false;
    short in_range = 0;
    float track_width = this->get_parameter("lap_track_width").as_double();
    for (auto &cone : this->cone_list)
    {

        if ((float)std::sqrt(cone.pos.x * cone.pos.x + cone.pos.y * cone.pos.y) <= track_width)
        {
            in_range++;
            if (cone.pos.CAMERA_HORIZONTAL_AXIS < 0)
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
        data_->clearConeList();
        cone_list.clear();
        laps++;
    }
}

bool LapCounter::isSameCone(const cone_data &old_cone, const cone_data &new_cone, float new_distance)
{
    float distance = sqrt(pow(new_cone.pos.x - old_cone.pos.x, 2) + pow(new_cone.pos.y - old_cone.pos.y, 2));

    return fabs(new_distance - last_callback_distance - distance) < ACCEPTABLE_CONE_DISTANCE_ERROR;
}

void LapCounter::resetLapCount()
{
    this->laps = -1;
    data_->setDistance(0);
}