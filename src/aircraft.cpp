#include "aircraft.hpp"

#include "GL/opengl_interface.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iterator>

void Aircraft::turn_to_waypoint()
{
    if (!waypoints.empty())
    {
        Point3D target = waypoints[0];
        if (waypoints.size() > 1)
        {
            const float d   = (waypoints[0] - pos).length();
            const Point3D W = (waypoints[0] - waypoints[1]).normalize(d / 2.0f);
            target += W;
        }

        turn(target - pos - speed);
    }
}

void Aircraft::turn(Point3D direction)
{
    (speed += direction.cap_length(type.max_accel)).cap_length(max_speed());
}

unsigned int Aircraft::get_speed_octant() const
{
    const float speed_len = speed.length();
    if (speed_len > 0)
    {
        const Point3D norm_speed { speed * (1.0f / speed_len) };
        const float angle =
            (norm_speed.y() > 0) ? 2.0f * 3.141592f - std::acos(norm_speed.x()) : std::acos(norm_speed.x());
        // partition into NUM_AIRCRAFT_TILES equal pieces
        return (static_cast<int>(std::round((angle * NUM_AIRCRAFT_TILES) / (2.0f * 3.141592f))) + 1) %
               NUM_AIRCRAFT_TILES;
    }
    else
    {
        return 0;
    }
}

// when we arrive at a terminal, signal the tower
void Aircraft::arrive_at_terminal()
{
    assert(is_on_ground() && has_terminal());
    // we arrived at a terminal, so start servicing
    control.arrived_at_terminal(*this);
    is_at_terminal = true;
}

// deploy and retract landing gear depending on next waypoints
void Aircraft::operate_landing_gear()
{
    if (waypoints.size() > 1u)
    {
        const auto it            = waypoints.begin();
        const bool ground_before = it->is_on_ground();
        const bool ground_after  = std::next(it)->is_on_ground();
        // deploy/retract landing gear when landing/lifting-off
        if (ground_before && !ground_after)
        {
            std::cout << flight_number << " lift off" << std::endl;
        }
        else if (!ground_before && ground_after)
        {
            std::cout << flight_number << " is now landing..." << std::endl;
            landing_gear_deployed = true;
        }
        else if (!ground_before && !ground_after)
        {
            landing_gear_deployed = false;
        }
    }
}

bool Aircraft::move()
{
    auto ret = true;

    if (is_circling() && !serve)
    {
        auto tmp_waypoints = control.reserve_terminal(*this);
        if (!tmp_waypoints.empty())
        {
            std::copy(tmp_waypoints.begin(), tmp_waypoints.end(), std::back_inserter(waypoints));
        }
    }

    if (waypoints.empty())
    {
        constexpr auto front = false;
        for (const auto& wp : control.get_instructions(*this))
        {
            add_waypoint<front>(wp);
        }
    }

    if (!is_at_terminal)
    {
        if (waypoints.empty())
        {
            ret = false;
        }

        turn_to_waypoint();
        // move in the direction of the current speed
        pos += speed;

        // if we are close to our next waypoint, stike if off the list
        if (!waypoints.empty() && distance_to(waypoints.front()) < DISTANCE_THRESHOLD)
        {
            if (waypoints.front().is_at_terminal())
            {
                arrive_at_terminal();
                serve = true;
            }
            else
            {
                operate_landing_gear();
            }
            waypoints.pop_front();
        }

        if (is_on_ground())
        {
            if (!landing_gear_deployed)
            {
                using namespace std::string_literals;
                throw AircraftCrash { flight_number + " crashed into the ground"s };
            }
        }
        else
        {
            // if we are in the air, but too slow, then we will sink!
            const float speed_len = speed.length();

            if (current_fuel > 0)
            {
                current_fuel -= 1;
            }

            if (speed_len < SPEED_THRESHOLD)
            {
                pos.z() -= SINK_FACTOR * (SPEED_THRESHOLD - speed_len);
            }
        }

        // update the z-value of the displayable structure
        GL::Displayable::z = pos.x() + pos.y();
    }

    return ret;
}

void Aircraft::display() const
{
    type.texture.draw(project_2D(pos), { PLANE_TEXTURE_DIM, PLANE_TEXTURE_DIM }, get_speed_octant());
}

void Aircraft::init_fuel(int min, int max)
{
    std::srand(std::time(nullptr));
    current_fuel = std::rand() % (max - min + 1) + min;
}

int Aircraft::check_fuel() const
{
    if (current_fuel == 0)
    {
        using namespace std::string_literals;
        throw AircraftCrash { flight_number + " crashed because of fuel issue"s };
    }
    return current_fuel;
}

bool Aircraft::has_terminal() const
{
    return !waypoints.empty() && waypoints.back().is_at_terminal();
}

bool Aircraft::is_circling() const
{
    return !has_terminal() && !is_at_terminal;
}

bool Aircraft::is_low_on_fuel() const
{
    return current_fuel < min_fuel;
}

int Aircraft::get_max_fuel()
{
    return max_fuel;
}

int Aircraft::get_min_fuel()
{
    return min_fuel;
}

bool Aircraft::check_is_at_terminal() const
{
    return is_at_terminal;
}

void Aircraft::refill(int& fuel_stock)
{
    assert(is_low_on_fuel());
    auto need = max_fuel - check_fuel();
    if (need > fuel_stock)
    {
        need = fuel_stock;
    }

    current_fuel += need;
    fuel_stock -= need;
    std::cout << need << " L of fuel were used for " << flight_number << std::endl;
}