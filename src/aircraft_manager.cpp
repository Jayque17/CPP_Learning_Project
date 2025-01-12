#include "aircraft_manager.hpp"

#include <algorithm>
#include <numeric>

void AircraftManager::add(std::unique_ptr<Aircraft> aircraft)
{
    assert(aircraft);
    aircrafts.emplace_back(std::move(aircraft));
}

bool AircraftManager::move()
{
    std::stable_sort(aircrafts.begin(), aircrafts.end(), compare_reserved_terminal_then_fuel_quantity);

    aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                                   [this](const std::unique_ptr<Aircraft>& aircraft)
                                   {
                                       bool check_move;
                                       try
                                       {
                                           check_move = !(*aircraft).move();
                                           (*aircraft).check_fuel();

                                       } catch (const AircraftCrash& error)
                                       {
                                           std::cerr << error.what() << std::endl;
                                           aircraft_crash_on_ground += 1;
                                           return true;
                                       }
                                       return check_move;
                                   }),
                    aircrafts.end());

    return true;
}

void AircraftManager::display_aircrafts_by_airlines(int airline, std::string airline_name)
{
    auto nb = std::count_if(aircrafts.begin(), aircrafts.end(),
                            [airline, airline_name](const std::unique_ptr<Aircraft>& aircraft)
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

int AircraftManager::get_required_fuel()
{
    return std::transform_reduce(
        aircrafts.begin(), aircrafts.end(), 0, [](int val1, int val2) { return val1 + val2; },
        [](const std::unique_ptr<Aircraft>& aircraft)
        {
            if ((*aircraft).check_fuel() < (*aircraft).get_min_fuel() && (*aircraft).check_is_at_terminal())
            {
                return (*aircraft).get_max_fuel() - (*aircraft).check_fuel();
            }

            return 0;
        });
}

int AircraftManager::get_crash() const
{
    return aircraft_crash_on_ground;
}