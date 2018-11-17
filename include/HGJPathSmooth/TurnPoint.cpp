//
// Created by stumbo on 18-11-5.
//

#include <cmath>
#include "TurnPoint.h"

HGJ::TurnPoint_s::TurnPoint_s(const HGJ::vec3f & p0, const HGJ::vec3f & p1,
                              const HGJ::vec3f & p2)
        : p0(p0), p1(p1), p2(p2),
          lenBefore((p1 - p0).len()),
          lenAfter((p2 - p1).len()),
          t1(p1 - p0 / lenBefore),
          t2(p2 - p1 / lenAfter),
          beta(acos(t1 * t2) / 2),
          yita(6 * c3 * cos(beta) / (c1 + 4))
{
    double cb = cos(beta);
    coeff_d2Kmax = cb * cb / (c5 * sin(beta));
}

