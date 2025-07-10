
#include "rclcpp/rclcpp.hpp"
#include "lap_counter/GetCones.hpp"
#include "lap_counter/GetPulse.hpp"
#include "lap_counter/LapCounter.hpp"
#include "lap_counter/DataHolder.hpp"
#include "lap_counter/GetMission.hpp"

int main(int argc, char *argv[])
{       
    auto data = std::make_shared<DataHolder>();

    rclcpp::init(argc, argv);

    auto lap_counter_node = std::make_shared<LapCounter>(data);
    auto get_cones_node = std::make_shared<GetCones>(data);
    auto get_pulse_node = std::make_shared<GetPulse>(data);
    auto get_mission_node = std::make_shared<GetMission>(data, lap_counter_node);

    rclcpp::executors::MultiThreadedExecutor node_spinner;
    node_spinner.add_node(lap_counter_node);
    node_spinner.add_node(get_cones_node);
    node_spinner.add_node(get_pulse_node);
    node_spinner.add_node(get_mission_node);

    node_spinner.spin();
    rclcpp::shutdown();
    return 0;
}