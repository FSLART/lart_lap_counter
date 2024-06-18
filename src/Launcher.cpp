
#include "rclcpp/rclcpp.hpp"
#include "lap_counter/GetCones.hpp"
#include "lap_counter/GetPulse.hpp"
#include "lap_counter/LapCounter.hpp"
#include "lap_counter/DataHolder.hpp"

int main(int argc, char *argv[])
{
    
    auto data = std::make_shared<DataHolder>();

    rclcpp::init(argc, argv);
    auto lap_counter_node = std::make_shared<LapCounter>(data);
    auto get_cones_node = std::make_shared<GetCones>(data);
    auto get_pulse_node = std::make_shared<GetPulse>(data);

    rclcpp::executors::MultiThreadedExecutor nodeSpinner;
    nodeSpinner.add_node(lap_counter_node);
    nodeSpinner.add_node(get_cones_node);
    nodeSpinner.add_node(get_pulse_node);

    nodeSpinner.spin();
    rclcpp::shutdown();
    return 0;
}