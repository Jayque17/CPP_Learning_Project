#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std::string_literals;

TowerSimulation::TowerSimulation(int argc, char** argv) :
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) },
    context_initializer { argc, argv }
{

    create_keystrokes();
    GL::move_queue.emplace(&aircraft_manager);
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}

void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]()
                           { aircraft_manager.add(aircraft_factory.create_random_aircraft(airport)); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    GL::keystrokes.emplace('u', []() { GL::speedUp(); });
    GL::keystrokes.emplace('d', []() { GL::speedDown(); });
    GL::keystrokes.emplace('p', []() { GL::stop(); });
    GL::keystrokes.emplace(
        '0', [this]()
        { aircraft_manager.display_aircrafts_by_airlines(0, aircraft_factory.get_airline_name(0)); });
    GL::keystrokes.emplace(
        '1', [this]()
        { aircraft_manager.display_aircrafts_by_airlines(1, aircraft_factory.get_airline_name(1)); });
    GL::keystrokes.emplace(
        '2', [this]()
        { aircraft_manager.display_aircrafts_by_airlines(2, aircraft_factory.get_airline_name(2)); });
    GL::keystrokes.emplace(
        '3', [this]()
        { aircraft_manager.display_aircrafts_by_airlines(3, aircraft_factory.get_airline_name(3)); });
    GL::keystrokes.emplace(
        '4', [this]()
        { aircraft_manager.display_aircrafts_by_airlines(4, aircraft_factory.get_airline_name(4)); });
    GL::keystrokes.emplace(
        '5', [this]()
        { aircraft_manager.display_aircrafts_by_airlines(5, aircraft_factory.get_airline_name(5)); });
    GL::keystrokes.emplace(
        '6', [this]()
        { aircraft_manager.display_aircrafts_by_airlines(6, aircraft_factory.get_airline_name(6)); });
    GL::keystrokes.emplace(
        '7', [this]()
        { aircraft_manager.display_aircrafts_by_airlines(7, aircraft_factory.get_airline_name(7)); });
    GL::keystrokes.emplace('m', [this]() { display_crash_counter(); });
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    for (const auto& [key, value] : GL::keystrokes)
    {
        std::cout << key << ' ';
    }

    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    airport = new Airport { one_lane_airport, aircraft_manager, Point3D { 0, 0, 0 },
                            new img::Image { one_lane_airport_sprite_path.get_full_path() } };
    GL::move_queue.emplace(airport);
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();
    assert(airport);

    GL::loop();
}

void TowerSimulation::display_crash_counter()
{
    std::cout << "Aircrafts which crashed into the ground: " << aircraft_manager.get_crash() << std::endl
              << "Aircrafts which crashed because of fuel issue: " << airport->get_tower().get_crash()
              << std::endl;
}