#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <ostream>

template <int NbCoord, typename NumericType> class Point
{
private:
    std::array<NumericType, NbCoord> values;

public:
    Point() {}
    Point(NumericType coords...) : values { coords } {}

    NumericType& selected_coord(int position) { return values[position]; }
    // NumericType const selected_coord(int position) { return values[position]; }

    std::array<NumericType, NbCoord>& get_values() { return values; }
    std::array<NumericType, NbCoord> get_values() const { return values; }

    Point<NbCoord, NumericType>& operator+=(const Point<NbCoord, NumericType>& other)
    {
        std::array<NumericType, NbCoord> new_values;
        std::transform(values.begin(), values.end(), other.get_values().begin(), new_values.begin(),
                       [](NumericType& val1, NumericType& val2) { return val1 + val2; });
        values = new_values;
        return *this;
    };

    Point<NbCoord, NumericType>& operator-=(const Point<NbCoord, NumericType>& other)
    {
        std::array<NumericType, NbCoord> new_values;
        std::transform(values.begin(), values.end(), other.get_values().begin(), new_values.begin(),
                       [](NumericType& val1, NumericType& val2) { return val1 - val2; });
        values = new_values;
        return *this;
    };

    Point<NbCoord, NumericType>& operator*=(const NumericType scalar)
    {
        std::for_each(values.begin(), values.end(), [scalar](NumericType& val) { val *= scalar; });
        return *this;
    };

    Point<NbCoord, NumericType>& operator*=(const Point<NbCoord, NumericType>& other)
    {
        std::array<NumericType, NbCoord> new_values;
        std::transform(values.begin(), values.end(), other.get_values().begin(), new_values.begin(),
                       [](NumericType& val1, NumericType& val2) { return val1 * val2; });
        values = new_values;
        return *this;
    };

    Point<NbCoord, NumericType> operator+(const Point<NbCoord, NumericType>& other) const
    {
        Point<NbCoord, NumericType> result = *this;
        result += other;
        return result;
    };

    Point<NbCoord, NumericType> operator-(const Point<NbCoord, NumericType>& other) const
    {
        Point<NbCoord, NumericType> result = *this;
        result -= other;
        return result;
    };

    Point<NbCoord, NumericType> operator*(const Point<NbCoord, NumericType>& other) const
    {
        Point<NbCoord, NumericType> result = *this;
        result *= other;
        return result;
    };

    Point<NbCoord, NumericType> operator*(const NumericType scalar) const
    {
        Point<NbCoord, NumericType> result = *this;
        result *= scalar;
        return result;
    };

    Point<NbCoord, NumericType> operator-() { return Point<NbCoord, NumericType> {}; };

    float length() const
    {
        return std::sqrt(std::transform_reduce(
            values.begin(), values.end(), 0.0, [](NumericType val1, NumericType val2) { return val1 + val2; },
            [](NumericType val) { return val * val; }));
    };
    float distance_to(const Point<NbCoord, NumericType>& other) const { return (*this - other).length(); };

    Point<NbCoord, NumericType>& normalize() {};
    Point<NbCoord, NumericType>& cap_lentgh() {};

    std::ostream& operator<<(const Point<NbCoord, NumericType>& point)
    {
        std::ostream os;
        os << "(";
        std::for_each(point.get_values().begin(), point.get_values().last(),
                      [os](NumericType val) { os << val << " "; });
        os << ")" << std::endl;
        return os;
    }
};

void test_generic_points()
{
    Point<3, int> p1(1, 2, 3);
    Point<3, int> p2(4, 5, 6);
    auto p3 = p1 + p2;
    p1 += p2;
    p1 *= 3;

    std::cout << p1.length() << std::endl;
    std::cout << p2.length() << std::endl;
    std::cout << p3.length() << std::endl;
};
