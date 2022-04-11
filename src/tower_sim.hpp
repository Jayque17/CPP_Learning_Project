#pragma once

#include "aircraft_factory.hpp"
#include "aircraft_manager.hpp"

#include <memory>

class Airport;
class Aircraft;

struct ContextInitializer
{
    int argc;
    char** argv;

    ContextInitializer(int argc_, char** argv_) : argc { argc_ }, argv { argv_ }
    {
        MediaPath::initialize(argv[0]);
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        GL::init_gl(argc, argv, "Airport Tower Simulation");
    };
};

class TowerSimulation
{
private:
    AircraftManager aircraft_manager;
    bool help        = false;
    Airport* airport = nullptr;
    ContextInitializer context_initializer;
    AircraftFactory aircraft_factory;

    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    void create_keystrokes();
    void display_help() const;

    void init_airport();

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch();
};
