//
// Created by stumbo on 18-11-5.
//

#ifndef HELLOCPLEX_VEC3F_H
#define HELLOCPLEX_VEC3F_H

#include <ostream>

namespace HGJ {
    class vec3f;

    /// for LR able to swap, moving function out of the member function
    vec3f operator* (double d, const vec3f & b);
    /// for LR able to swap, moving function out of the member function
    vec3f operator* (const vec3f & b, double d);

    std::ostream & operator<< (std::ostream & os, const vec3f & v);
}

class HGJ::vec3f{

    friend vec3f operator* (double d, const vec3f & b);
    friend std::ostream & operator<< (std::ostream & os, const vec3f & v);

private:
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

public:
    vec3f() = default;

    vec3f(double x, double y, double z);

    vec3f operator- (const vec3f& b) const;

    vec3f operator+ (const vec3f& b) const;

    double operator* (const vec3f & b) const;

    vec3f operator/ (const double& b) const;

    vec3f& operator/=(double b) noexcept;

    vec3f& operator*=(double b) noexcept;

    vec3f operator+=(const vec3f & b) noexcept;

    double len() const;
};


#endif //HELLOCPLEX_VEC3F_H
