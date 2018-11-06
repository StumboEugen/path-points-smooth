//
// Created by stumbo on 18-11-5.
//

#include "vec3f.h"
#include <cmath>

HGJ::vec3f::vec3f(double x, double y, double z)
        : x(x), y(y), z(z)
{}

HGJ::vec3f HGJ::vec3f::operator-(const HGJ::vec3f & b) const {
    vec3f res{};
    res.x = this->x - b.x;
    res.y = this->y - b.y;
    res.z = this->z - b.z;
    return res;
}

HGJ::vec3f HGJ::vec3f::operator+(const HGJ::vec3f & b) const {
    vec3f res{};
    res.x = this->x + b.x;
    res.y = this->y + b.y;
    res.z = this->z + b.z;
    return res;
}

HGJ::vec3f HGJ::vec3f::operator*(const double & b) const {
    vec3f res;
    res.x = this->x * b;
    res.y = this->y * b;
    res.z = this->z * b;
    return res;
}

HGJ::vec3f HGJ::vec3f::operator/(const double & b) const {
    vec3f res;
    res.x = this->x / b;
    res.y = this->y / b;
    res.z = this->z / b;
    return res;
}

double HGJ::vec3f::len() const {
    return sqrt(x * x + y * y + z * z);
}

HGJ::vec3f & HGJ::vec3f::operator/=(double b) {
    x /= b;
    y /= b;
    z /= b;
    return *this;
}

HGJ::vec3f & HGJ::vec3f::operator*=(double b) {
    x *= b;
    y *= b;
    z *= b;
    return *this;
}


