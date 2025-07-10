
#include "lap_counter/LapCounter.hpp"
#include "lart_msgs/msg/mission.hpp"

using namespace std::chrono_literals;

bool compare_cone(const cone_data &first, const cone_data &second)
{
    return sqrt(first.pos.x * first.pos.x + first.pos.y * first.pos.y) > sqrt(second.pos.x * second.pos.x + second.pos.y * second.pos.y);
}

LapCounter::LapCounter(std::shared_ptr<DataHolder> data)
    : Node("count_laps"), data_(data), distance_after_lap(-1), last_callback_distance(0.0), laps(-1)
{
    ekf_subscriber_ = this->create_subscription<geometry_msgs::msg::PoseStamped>(
    "/ekf/state", 10, std::bind(&LapCounter::poseCallback, this, std::placeholders::_1));
    
    // ros args
    this->declare_parameter("lap_min", rclcpp::ParameterValue((float)DEFAULT_DISTANCES_MIN));
    this->declare_parameter("lap_max", rclcpp::ParameterValue((float)DEFAULT_DISTANCES_MAX));
    this->declare_parameter("lap_track_width", rclcpp::ParameterValue((float)TRACK_WIDTH));
    this->declare_parameter("lap_position_margin", rclcpp::ParameterValue(1.0f));
    this->declare_parameter("lap_position_margin_y", rclcpp::ParameterValue(3.0f));

    publisher_ = this->create_publisher<LAP_PUBLISHER_TYPE>(LAP_PUBLISHER_NAME, 5);
    timer_ = this->create_wall_timer(PUBLISHER_TIMER, std::bind(&LapCounter::topicCallback, this));
}

void LapCounter::topicCallback()
{
    verifyLap();

    auto laps = std_msgs::msg::UInt16();
    laps.data = this->laps == -1 ? 0 : this->laps;

    RCLCPP_INFO(this->get_logger(), "laps: %d", this->laps);

    publisher_->publish(laps);
}


void LapCounter::poseCallback(const geometry_msgs::msg::PoseStamped::SharedPtr msg)
{
    last_pose_ = *msg;
    float x = last_pose_.pose.position.x;
    float y = last_pose_.pose.position.y;
    data_->setMission(lart_msgs::msg::Mission::ACCELERATION);
    
    int mission = data_->getMission();

    RCLCPP_INFO(this->get_logger(), "Pose EKF recebida: x = %.2f, y = %.2f (Missão: %d)", x, y, mission);
}


void LapCounter::verifyLap()
{
    data_->setMission(lart_msgs::msg::Mission::ACCELERATION);
    int mission = data_->getMission();
    float margin_x = this->get_parameter("lap_position_margin").as_double();
    float margin_y = this->get_parameter("lap_position_margin_y").as_double();

    float x = last_pose_.pose.position.x;
    float y = last_pose_.pose.position.y;


    if (laps == -1) {
        if (mission == lart_msgs::msg::Mission::SKIDPAD) {
            if (fabs(x - 15.0) < margin_x) {
                laps = 0;
                distance_after_lap = data_->getDistance();
                RCLCPP_INFO(this->get_logger(), "Contagem de voltas iniciada (SKIDPAD)!");
                return;  // não conta volta já aqui
            }
        } else {
            // Nas outras missões, inicia imediatamente
            laps = 0;
            distance_after_lap = data_->getDistance();
            RCLCPP_INFO(this->get_logger(), "Contagem de voltas iniciada!");
        }
    }

    // Impede múltiplos incrementos seguidos
    if (distance_after_lap > -1) {
        float delta_distance = data_->getDistance() - distance_after_lap;
        if (delta_distance < SAFE_LAP_DISTANCE) {
            return;
        }
    }

    bool lap_complete = false;

    switch (mission) {
        case lart_msgs::msg::Mission::ACCELERATION:
            if (fabs(x - 75.0) < margin_x) lap_complete = true;
            break;
        case lart_msgs::msg::Mission::SKIDPAD:
            if (fabs(x - 15.0) < margin_x) lap_complete = true;
            break;
        case lart_msgs::msg::Mission::TRACKDRIVE:
        case lart_msgs::msg::Mission::AUTOCROSS:
            if (fabs(x) < margin_x && fabs(y) < margin_y) lap_complete = true;
            break;
        default:
            break;
    }

    if (lap_complete) {
        laps++;
        distance_after_lap = data_->getDistance();
        RCLCPP_INFO(this->get_logger(), "Volta concluída! Total de voltas: %d", laps);
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