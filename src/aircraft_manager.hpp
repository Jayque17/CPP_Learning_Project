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
    static bool compare_reserved_terminal_then_fuel_quantity(const std::unique_ptr<Aircraft>& aircraft1,
                                                             const std::unique_ptr<Aircraft>& aircraft2);

public:
    void add(std::unique_ptr<Aircraft> aircraft);
    bool move() override;
    void display_aircrafts_by_airlines(int airline, std::string airline_name);
    int get_required_fuel();
};