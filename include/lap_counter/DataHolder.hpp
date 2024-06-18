#ifndef DATA_HOLDER_HPP_
#define DATA_HOLDER_HPP_

#include <iostream>
#include <mutex>
#include <memory>

#include "lap_counter/constants.hpp"

class DataHolder
{
public:
    DataHolder();

    // distance
    float getDistance();
    void setDistance(float data);

    // cones
    std::unique_ptr<cone_node> getConeListHeader();
    void setConeListHeader(std::unique_ptr<cone_node> data);

private:
    float distance;
    std::unique_ptr<cone_node> coneListHeader;
    std::mutex velocity_mutex, list_mutex;
};

#endif /* DATA_HOLDER_HPP_ */