#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#define PI 3.1415926535897932384626433832795 // 32 digits of PI
#define RADIUS 0.255                         // radius of the wheel in meters
#define PULSES_PER_REVOLUTION 35             // pulses per full wheel rotation
#define METERS_PER_REVOLUTION 2 * PI *RADIUS // distance travelled per wheel rotation

// Publishers and subscribers
#define PUBLISHER_TIMER 100ms                          // frequency of the publisher
#define LAP_PUBLISHER_TYPE std_msgs::msg::Int32        // message type of the lap counter publisher
#define CONE_SUBSCRIBER_TYPE lart_msgs::msg::ConeArray // message type of the cone topic subscriber
#define PULSE_SUBSCRIBER_TYPE lart_msgs::msg::Dynamics // message type of the pulse topic subscriber

#define CONE_TOPIC_NAME "/cone_array_topic"
#define PULSE_TOPIC_NAME "/pulse_count"

// Cones
typedef struct
{
    double x;
    double y;
} position;

struct cone_node
{
    position data;
    std::unique_ptr<cone_node> next;
};

#define CONE_LARGE_ORANGE_TYPE 4

#endif /* CONSTANTS_HPP_ */