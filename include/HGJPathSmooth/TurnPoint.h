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
        //p1 is the current point, p0 is the last, p2 is the next
        vec3f p0, p1, p2;
        // length of p0p1
        double lenBefore;
        // length of p1p2
        double lenAfter;
        // p0 to p1 unit vector
        vec3f t1;
        // p1 to p2 unit vector
        vec3f t2;
        // half of the angle theta(angle of t1 t2)
        double beta;
        double yita;
        // the curve extend
        double d = 0.0;
        // the max spd on this curve
        double maxSpd = 0;
        // the spd on this curve
        double speed = 0.0;
        // d * coeff_d2Kmax = Kmax
        double coeff_d2Kmax;

        TurnPoint_s(const vec3f & p0, const vec3f & p1, const vec3f & p2);

        void completeSpdModify() {
            spdModifiedComplete = true;
        }

        bool isSpdModifiedComplete() const {
            return spdModifiedComplete;
        }
    private:
        // if the speed has modified complete
        bool spdModifiedComplete = false;

    } TurnPoint;
}


#endif //HELLOCPLEX_TURNPOINT_H
