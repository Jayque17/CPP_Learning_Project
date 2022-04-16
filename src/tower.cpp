#include "tower.hpp"

#include "airport.hpp"
#include "terminal.hpp"

#include <algorithm>
#include <cassert>

WaypointQueue Tower::get_circle() const
{
    return { { Point3D { -1.5f, -1.5f, .5f }, wp_air },
             { Point3D { 1.5f, -1.5f, .5f }, wp_air },
             { Point3D { 1.5f, 1.5f, .5f }, wp_air },
             { Point3D { -1.5f, 1.5f, .5f }, wp_air } };
}

WaypointQueue Tower::get_instructions(Aircraft& aircraft)
{
    if (!aircraft.is_at_terminal)
    {
        // if the aircraft is far, then just guide it to the airport vicinity
        if (aircraft.distance_to(airport.pos) < 5)
        {
            // try and reserve a terminal for the craft to land
            const auto vp = airport.reserve_terminal(aircraft);
            if (!vp.first.empty())
            {
                reserved_terminals[&aircraft] = vp.second;
                return vp.first;
            }
            else
            {
                return get_circle();
            }
        }
        else
        {
            return {};
        }
    }
    else
    {
        // get a path for the craft to start
        const auto aircraft_and_term = find_craft_and_terminal(aircraft);
        assert(aircraft_and_term != reserved_terminals.end());
        const auto terminal_num = aircraft_and_term->second;
        Terminal& terminal      = airport.get_terminal(terminal_num);
        if (!terminal.is_servicing())
        {
            terminal.finish_service();
            reserved_terminals.erase(aircraft_and_term);
            aircraft.is_at_terminal = false;
            return airport.start_path(terminal_num);
        }
        else
        {
            return {};
        }
    }
}

void Tower::arrived_at_terminal(const Aircraft& aircraft)
{
    const auto it = find_craft_and_terminal(aircraft);
    assert(it != reserved_terminals.end());
    airport.get_terminal(it->second).start_service(aircraft);
}

WaypointQueue Tower::reserve_terminal(Aircraft& aircraft)
{ // if the aircraft is far, then just guide it to the airport vicinity
    if (aircraft.distance_to(airport.pos) < 5)
    {
        // try and reserve a terminal for the craft to land
        const auto vp = airport.reserve_terminal(aircraft);
        if (!vp.first.empty())
        {
            reserved_terminals[&aircraft] = vp.second;
            return vp.first;
        }
    }

    return {};
}

void Tower::free_terminal(std::string flight_number)
{
    if (!reserved_terminals.empty())
    {
        auto it = std::find_if(reserved_terminals.begin(), reserved_terminals.end(),
                               [flight_number](std::pair<const Aircraft* const, long unsigned int> rt)
                               {
                                   auto aircraft = rt.first;
                                   if (aircraft != nullptr)
                                   {
                                       return (*aircraft).get_flight_num().compare(flight_number) == 0;
                                   }
                                   return false;
                               });

        reserved_terminals.erase(it);
    }
}