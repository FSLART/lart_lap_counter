#ifndef LAP_COUNTER_HPP_
#define LAP_COUNTER_HPP_

#include <chrono>
#include <memory>
#include <cmath>

#include "lap_counter/constants.hpp"
#include "lart_common/lart_common.h"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/u_int16.hpp"
#include "lap_counter/DataHolder.hpp"

class LapCounter : public rclcpp::Node
{
public:
    LapCounter(std::shared_ptr<DataHolder> data);
    void resetLapCount();

private:
    // atributes
    const std::shared_ptr<DataHolder> data_;

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<LAP_PUBLISHER_TYPE>::SharedPtr publisher_;
    std::list<cone_data, std::allocator<cone_data>> cone_list;
    float last_distance_known[4];
    float distance_after_lap;
    int8_t laps;

    // methods
    void topicCallback();
    bool isSameCone(const cone_data& old_cone, const cone_data& new_cone, short index);
};

#endif /* LAP_COUNTER_HPP_ */