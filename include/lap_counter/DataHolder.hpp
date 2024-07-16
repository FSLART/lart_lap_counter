#ifndef DATA_HOLDER_HPP_
#define DATA_HOLDER_HPP_

#pragma region 'LIBS'
#include <iostream>
#include <list>
#include <mutex>
#include <memory>

#include "lap_counter/constants.hpp"
#pragma endregion

class DataHolder
{
public:
    DataHolder();

    // distance
    float getDistance();
    void setDistance(float data);

    // cones
    std::list<cone_data> getConeList();
    void setConeList(std::list<cone_data> data);

    // mission
    u_int8_t getMissionType();
    void setMissionType(u_int8_t data);

private:
    // atributes
    float distance;
    std::list<cone_data, std::allocator<cone_data>> cone_list;
    u_int8_t mission_type;

    std::mutex distance_mutex, list_mutex, mission_mutex;
};

#endif /* DATA_HOLDER_HPP_ */