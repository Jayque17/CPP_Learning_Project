#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

class AircraftManager : public GL::DynamicObject
{
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts = std::vector<std::unique_ptr<Aircraft>>();

public:
    void add(std::unique_ptr<Aircraft> aircraft);
    bool move() override;
    void display_aircrafts_by_airlines(int airline, std::string airline_name);
};