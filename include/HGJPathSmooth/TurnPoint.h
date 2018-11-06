//
// Created by stumbo on 18-11-5.
//

#ifndef HELLOCPLEX_TURNPOINT_H
#define HELLOCPLEX_TURNPOINT_H

#include "tools.h"
#include "vec3f.h"

namespace HGJ {
    typedef struct TurnPoint_s {
        double d = 0.0;
        double speed = 0.0;
        vec3f t1;
        vec3f t2;
        TurnPoint_s(const vec3f & p0, const vec3f & p1, const vec3f & p2);
    } TurnPoint;
}


#endif //HELLOCPLEX_TURNPOINT_H
