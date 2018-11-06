//
// Created by stumbo on 18-11-5.
//

#ifndef HELLOCPLEX_VEC3F_H
#define HELLOCPLEX_VEC3F_H

namespace HGJ {
    class vec3f;
}

class HGJ::vec3f{

private:
    double x;
    double y;
    double z;

public:
    explicit vec3f(double x = 0.0, double y = 0.0, double z = 0.0);

    vec3f operator- (const vec3f& b) const;

    vec3f operator+ (const vec3f& b) const;

    vec3f operator* (const double& b) const;

    vec3f operator/ (const double& b) const;

    vec3f& operator/=(double b);

    vec3f& operator*=(double b);

    double len() const;
};


#endif //HELLOCPLEX_VEC3F_H
