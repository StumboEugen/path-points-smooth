//
// Created by stumbo on 18-11-5.
//

#ifndef HELLOCPLEX_TURNPOINT_H
#define HELLOCPLEX_TURNPOINT_H

#include "tools.h"
#include "vec3f.h"

namespace HGJ {
    typedef struct TurnPoint_s {
        /**
         * the sequence of these paramaters CAN NOT be changed, for correct init
         */
        // length of p0p1
        double len1;
        // length of p1p2
        double len2;
        // p0 to p1 unit vector
        vec3f t1;
        // p1 to p2 unit vector
        vec3f t2;
        // half of the angle theta(angle of t1 t2)
        double beta;
        double yita;
        // the curve extend
        double d = 0.0;
        // the spd on this curve
        double speed = 0.0;

        TurnPoint_s(const vec3f & p0, const vec3f & p1, const vec3f & p2);
        double coeff_d2Kmax() const;
    } TurnPoint;
}


#endif //HELLOCPLEX_TURNPOINT_H
