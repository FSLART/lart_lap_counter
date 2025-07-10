
#include "lap_counter/DataHolder.hpp"


DataHolder::DataHolder() : distance(0.0), cone_list(std::allocator<cone_data>()) {}


float DataHolder::getDistance()
{
    std::lock_guard<std::mutex> lock(distance_mutex);
    return distance;
}

void DataHolder::setDistance(float data)
{
    std::lock_guard<std::mutex> lock(distance_mutex);
    distance = data;
}

std::list<cone_data> DataHolder::getConeList()
{
    std::lock_guard<std::mutex> lock(list_mutex);
    return cone_list;
}

void DataHolder::setConeList(std::list<cone_data> data)
{
    std::lock_guard<std::mutex> lock(list_mutex);
    cone_list = std::move(data);
}

void DataHolder::setMission(u_int8_t mission)
{
    std::lock_guard<std::mutex> lock(mission_mutex);
    mission_type = mission;
}

u_int8_t DataHolder::getMission()
{
    std::lock_guard<std::mutex> lock(mission_mutex);
    return mission_type;
}

void DataHolder::clearConeList()
{
    std::lock_guard<std::mutex> lock(list_mutex);
    cone_list.clear();
}