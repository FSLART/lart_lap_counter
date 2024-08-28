#ifndef GET_CONES_HPP_
#define GET_CONES_HPP_

#include <memory>

#include "lap_counter/constants.hpp"
#include "lart_common/lart_common.h"
#include "rclcpp/rclcpp.hpp"
#include "lart_msgs/msg/cone_array.hpp"
#include "lap_counter/DataHolder.hpp"

class GetCones : public rclcpp::Node
{
public:
    GetCones(std::shared_ptr<DataHolder> data);

private:
    // atributes
    std::shared_ptr<DataHolder> data_;
    rclcpp::Subscription<CONE_SUBSCRIBER_TYPE>::SharedPtr subscriber_;

    // methods
    void topicCallback(const CONE_SUBSCRIBER_TYPE::SharedPtr msg) const;
};

#endif /* GET_CONES_HPP_ */