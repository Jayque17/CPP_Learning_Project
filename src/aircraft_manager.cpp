#include "aircraft_manager.hpp"

#include <algorithm>

void AircraftManager::add(std::unique_ptr<Aircraft> aircraft)
{
    aircrafts.emplace_back(std::move(aircraft));
}

bool AircraftManager::move()
{
    aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                                   [this](const std::unique_ptr<Aircraft>& aircraft)
                                   { return !(*aircraft).move(); }),
                    aircrafts.end());

    return true;
}

void AircraftManager::display_aircrafts_by_airlines(int airline, std::string airline_name)
{
    auto nb = std::count_if(aircrafts.begin(), aircrafts.end(),
                            [airline_name, airline](const std::unique_ptr<Aircraft>& aircraft)
                            { return aircraft->get_flight_num().find(airline_name) != std::string::npos; });
    std::cout << airline_name << " : " << nb << std::endl;
}