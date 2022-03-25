#pragma once

#include "aircraft.hpp"

#include <map>
#include <memory>
#include <string>

class AircraftManager
{
private:
    std::map<std::string, std::unique_ptr<Aircraft>> aircrafts =
        std::map<std::string, std::unique_ptr<Aircraft>>();

public:
    AircraftManager() {}
};