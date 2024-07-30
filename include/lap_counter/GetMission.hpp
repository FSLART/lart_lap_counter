#ifndef GET_MISSION_HPP_
#define GET_MISSION_HPP_

#include <memory>

#include "lap_counter/constants.hpp"
#include "lart_common/lart_common.h"
#include "rclcpp/rclcpp.hpp"
#include "lart_msgs/msg/as_status.hpp"
#include "lap_counter/DataHolder.hpp"
#include "lap_counter/LapCounter.hpp"

class GetMission : public rclcpp::Node
{
public:
    GetMission (std::shared_ptr<DataHolder> data_, std::shared_ptr<LapCounter> publisher_node_);

private:
    // atributes
    std::shared_ptr<DataHolder> data_;
    rclcpp::Subscription<MISSION_SUBSCRIBER_TYPE>::SharedPtr subscriber_;
    std::shared_ptr<LapCounter> lap_counter_node;
    uint8_t last_mission;

    // methods
    void topicCallback(const MISSION_SUBSCRIBER_TYPE::SharedPtr msg) const;
    mutable u_int8_t mission_type;
};

#endif /* GET_MISSION_HPP_ */