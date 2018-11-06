//
// Created by stumbo on 18-11-5.
//

#include <cmath>
#include "TurnPoint.h"

HGJ::TurnPoint_s::TurnPoint_s(const HGJ::vec3f & p0, const HGJ::vec3f & p1,
                              const HGJ::vec3f & p2)
        : len1((p1 - p0).len()),
          len2((p2 - p1).len()),
          t1(p1 - p0 / len1),
          t2(p2 - p1 / len2),
          beta(acos(t1 * t2) / 2),
          yita(6 * c3 * cos(beta) / (c1 + 4))
{

}

double HGJ::TurnPoint_s::coeff_d2Kmax() {
    return pow(cos(beta), 2) / (c5 * sin(beta));
}
