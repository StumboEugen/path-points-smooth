//
// Created by stumbo on 18-11-5.
//

#include <cmath>
#include <algorithm>
#include <iostream>
#include "TurnPoint.h"

using namespace std;

HGJ::TurnPoint_s::TurnPoint_s(const HGJ::vec3f & p0, const HGJ::vec3f & p1,
                              const HGJ::vec3f & p2)
        : p0(p0), p1(p1), p2(p2),
          lenBefore((p1 - p0).len()),
          lenAfter((p2 - p1).len()),
          t1((p1 - p0) / lenBefore),
          t2((p2 - p1) / lenAfter),
          beta(acos(t1 * t2) / 2),
          yita(6 * c3 * cos(beta) / (c1 + 4))
{
    double cb = cos(beta);
    coeff_d2Kmax = cb * cb / (c5 * sin(beta));
}

HGJ::vec3f HGJ::TurnPoint_s::calPoint(double u, bool partOne) {
    if (u < 0 || u > 1.0) {
        cerr << "[TurnPoint_s::calPoint] the paramater u:" << u << "is not valid!";
        if (u < 0) {
            u = 0.0;
        } else {
            u = 1.0;
        }
    }
    double & u1 = u;
    double u2 = u * u;
    double u3 = u2 * u;
    double uu = 1.0 - u;
    double uu2 = uu * uu;
    double uu3 = uu2 * uu;
    vec3f point{0,0,0};
    if (partOne) {
        point += uu3 * B1[0];
        point += 3.0 * uu2 * u1 *B1[1];
        point += 3.0 * uu * u2 * B1[2];
        point += u3 * B1[3];
    } else {
        point += uu3 * B2[3];
        point += 3.0 * uu2 * u * B2[2];
        point += 3.0 * uu * u2 * B2[1];
        point += u3 * B2[0];
    }
    return point;
}

void HGJ::TurnPoint_s::setD(double d, double spdMax, double accMax) {
    TurnPoint::d = d;
    TurnPoint::maxSpd = min(sqrt(accMax * TurnPoint::d * TurnPoint::coeff_d2Kmax), spdMax);
    B1[0] = p1 - t1 * d;
    B1[1] = p1 - t1 * d * (1 - c1 * c3);
    B1[2] = p1 - t1 * d * (1 - c1 * c3 - c3);

    B2[0] = p1 + t2 * d;
    B2[1] = p1 + t2 * d * (1 - c1 * c3);
    B2[2] = p1 + t2 * d * (1 - c1 * c3 - c3);

    B1[3] = (B1[2] + B2[2]) / 2.0;
    B2[3] = B1[3];
}

