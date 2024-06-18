
#include "lap_counter/DataHolder.hpp"

DataHolder::DataHolder() : distance(0.0), coneListHeader(nullptr) {}

float DataHolder::getDistance()
{
    std::lock_guard<std::mutex> lock(velocity_mutex);
    return distance;
}

void DataHolder::setDistance(float data)
{
    std::lock_guard<std::mutex> lock(velocity_mutex);
    distance = data;
}

std::unique_ptr<cone_node> DataHolder::getConeListHeader()
{
    std::lock_guard<std::mutex> lock(list_mutex);
    return std::move(coneListHeader);
}

void DataHolder::setConeListHeader(std::unique_ptr<cone_node> data)
{
    std::lock_guard<std::mutex> lock(list_mutex);
    coneListHeader = std::move(data);
}