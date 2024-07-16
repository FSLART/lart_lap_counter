#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <memory>

// Default distances
#define DEFAULT_DISTANCES (float[]){40.0, 80.0} // in meters (default minimum and maximum distance to consider a lap)

// Default distances for missions
#define MISSION_1_DISTANCE (float[]){70.0, 80.0} // in meters (default minimum and maximum distance to consider a lap, in mission 1)
#define MISSION_2_DISTANCE (float[]){40.0, 60.0} // in meters (default minimum and maximum distance to consider a lap, in mission 2)
#define MISSION_3_DISTANCE (float[]){50.0, 100.0} // in meters (default minimum and maximum distance to consider a lap, in mission 3)


// Publishers and subscribers
#define PUBLISHER_TIMER 100ms                           // frequency of the publisher
#define LAP_PUBLISHER_TYPE std_msgs::msg::UInt16        // message type of the lap counter publisher
#define CONE_SUBSCRIBER_TYPE lart_msgs::msg::ConeArray  // message type of the cone topic subscriber
#define PULSE_SUBSCRIBER_TYPE lart_msgs::msg::Dynamics  // message type of the pulse topic subscriber
#define MISSION_SUBSCRIBER_TYPE lart_msgs::msg::Mission // message type of the mission topic subscriber

#define LAP_PUBLISHER_NAME "lap_count"
#define CONE_SUBSCRIBER_NAME "lap_cone_reader"
#define PULSE_SUBSCRIBER_NAME "lap_pulse_reader"
#define MISSION_SUBSCRIBER_NAME "lap_mission_reader"

#define CONE_TOPIC_NAME "/mapping/cones"
#define PULSE_TOPIC_NAME "/dynamics"
#define MISSION_TOPIC_NAME "/launch"

#define ACCEPTABLE_CONE_DISTANCE_ERROR 2.0 // in meters (the acceptable error when calculating if a cone is the same one in the past frame)
#define TRACK_WIDTH 3.0                    // in meters (the track width from one large_orange_cone to another in different sides)
#define SAFE_LAP_DISTANCE 15.0             // in meters (the distance to wait after adding a lap)

// Cones
typedef struct
{
    double x;
    double y;
} position;

struct cone_data
{
    position pos;
};

#endif /* CONSTANTS_HPP_ */