//
// Created by stumbo on 18-11-5.
//

#ifndef HELLOCPLEX_VEC3F_H
#define HELLOCPLEX_VEC3F_H

namespace HGJ {
    class vec3f;

    vec3f operator* (double d, const vec3f & b);
}

class HGJ::vec3f{

    friend vec3f operator* (double d, const vec3f & b);

private:
    double x;
    double y;
    double z;

public:
    vec3f(double x = 0.0, double y = 0.0, double z = 0.0);

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
