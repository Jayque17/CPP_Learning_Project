#pragma once

#include "aircraft_manager.hpp"

#include <memory>

class Airport;
struct AircraftType;
// class AircraftManager;
class Aircraft;

class TowerSimulation
{
private:
    AircraftManager aircraft_manager;
    bool help        = false;
    Airport* airport = nullptr;

    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    std::unique_ptr<Aircraft> create_aircraft(const AircraftType& type) const;
    std::unique_ptr<Aircraft> create_random_aircraft() const;

    void create_keystrokes();
    void display_help() const;

    void init_airport();

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch();
};
