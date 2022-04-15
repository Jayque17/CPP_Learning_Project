#include "aircraft_manager.hpp"

#include <algorithm>
#include <ostream>

void AircraftManager::add(std::unique_ptr<Aircraft> aircraft)
{
    aircrafts.emplace_back(std::move(aircraft));
}

bool AircraftManager::move()
{

    std::stable_sort(aircrafts.begin(), aircrafts.end(), compare_reserved_terminal_then_fuel_quantity);

    aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                                   [this](const std::unique_ptr<Aircraft>& aircraft)
                                   { return (!(*aircraft).move() || (*aircraft).check_fuel() == 0); }),
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

bool AircraftManager::compare_reserved_terminal_then_fuel_quantity(const std::unique_ptr<Aircraft>& aircraft1,
                                                                   const std::unique_ptr<Aircraft>& aircraft2)
{
    if (((*aircraft1).has_terminal() && (*aircraft2).has_terminal()) ||
        (!(*aircraft1).has_terminal() && !(*aircraft2).has_terminal()))
    {
        return (*aircraft1).check_fuel() < (*aircraft2).check_fuel();
    }

    if ((*aircraft1).has_terminal() && !(*aircraft2).has_terminal())
    {
        return true;
    }

    return false;
}