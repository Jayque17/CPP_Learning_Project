#pragma once

#include "waypoint.hpp"

#include <algorithm>
#include <map>
#include <unordered_map>
#include <utility>

class Airport;
class Aircraft;
class Terminal;

class Tower
{
private:
    using AircraftToTerminal      = std::map<const Aircraft*, size_t>;
    using AircraftAndTerminalIter = AircraftToTerminal::iterator;

    Airport& airport;
    // aircrafts may reserve a terminal
    // if so, we need to save the terminal number in order to liberate it when the craft leaves
    AircraftToTerminal reserved_terminals = {};

    WaypointQueue get_circle() const;

    AircraftAndTerminalIter find_craft_and_terminal(const Aircraft& aircraft)
    {
        return reserved_terminals.find(&aircraft);
    }

public:
    Tower(Airport& airport_) : airport { airport_ } {}

    // produce instructions for aircraft
    WaypointQueue get_instructions(Aircraft& aircraft);
    void arrived_at_terminal(const Aircraft& aircraft);
};
