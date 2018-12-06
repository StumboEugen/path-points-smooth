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

HGJ::vec3f & HGJ::vec3f::operator/=(double b) noexcept {
    x /= b;
    y /= b;
    z /= b;
    return *this;
}

HGJ::vec3f & HGJ::vec3f::operator*=(double b) noexcept {
    x *= b;
    y *= b;
    z *= b;
    return *this;
}

double HGJ::vec3f::operator* (const HGJ::vec3f & b) const {
    return x * b.x + y * b.y + z * b.z;
}

HGJ::vec3f HGJ::vec3f::operator+=(const HGJ::vec3f & b) noexcept {
    x += b.x;
    y += b.y;
    z += b.z;
    return *this;
}

HGJ::vec3f HGJ::operator*(double d, const HGJ::vec3f & b) {
    vec3f temp = b;
    temp.x *= d;
    temp.y *= d;
    temp.z *= d;
    return temp;
}

std::ostream & HGJ::operator<<(std::ostream & os, const HGJ::vec3f &v) {
    os << v.x << "," << v.y << "," << v.z;
    return os;
}

HGJ::vec3f HGJ::operator*(const HGJ::vec3f & b, double d) {
    return d * b;
}


