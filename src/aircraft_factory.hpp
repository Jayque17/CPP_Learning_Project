#pragma once

#include <memory>
#include <set>
#include <string>

class Airport;
class Aircraft;
struct AircraftType;

constexpr size_t NUM_AIRCRAFT_TYPES = 3;

class AircraftFactory
{

private:
    const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };
    AircraftType* aircraft_types[NUM_AIRCRAFT_TYPES] {};
    void init_aircraft_types();
    mutable std::set<std::string> flight_numbers {};

public:
    std::string get_airline_name(int airline);
    std::unique_ptr<Aircraft> create_aircraft(Airport* airport, const AircraftType& type) const;
    std::unique_ptr<Aircraft> create_random_aircraft(Airport* airport) const;
    AircraftFactory() { init_aircraft_types(); };
};