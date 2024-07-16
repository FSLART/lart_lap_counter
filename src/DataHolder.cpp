
#include "lap_counter/DataHolder.hpp"

#pragma region 'PUBLIC'

DataHolder::DataHolder() : distance(0.0), cone_list(std::allocator<cone_data>()) {}

#pragma endregion


#pragma region 'PRIVATE'

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

u_int8_t DataHolder::getMissionType()
{
    std::lock_guard<std::mutex> lock(list_mutex);
    return mission_type;
}

void DataHolder::setMissionType(u_int8_t data)
{
    std::lock_guard<std::mutex> lock(list_mutex);
    mission_type = data;
}

#pragma endregion