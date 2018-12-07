//
// Created by stumbo on 18-11-5.
//

#ifndef HELLOCPLEX_TURNPOINT_H
#define HELLOCPLEX_TURNPOINT_H

#include "tools.h"
#include "vec3f.h"

namespace HGJ {
    class pathPlanner;

    typedef struct TurnPoint_s {
        /**
         * the sequence of these paramaters CAN NOT be changed, for correct init
         */
        /// p1 is the current point, p0 is the last, p2 is the next
        vec3f p0, p1, p2;
        /// length of p0p1
        double lenBefore;
        /// length of p1p2
        double lenAfter;
        /// p0 to p1 unit vector
        vec3f t1;
        /// p1 to p2 unit vector
        vec3f t2;
        /// half of the angle theta(angle of t1 t2)
        double beta;
        double yita;
        /// the curve extend
        double d = 0.0;
        /// the max spd on this curve
        double maxSpd = 0;
        /// the spd on this curve
        double speed = 0.0;
        /// d * coeff_d2Rmin = Kmax
        double coeff_d2Rmin;
        /// the half len of the curve, it's not accurate(smaller than real)
        double halfLength = 0.0;

        /// the bezier controll points, 0 connect the line, 4 connect the other curve
        vec3f B1[4];
        /// the bezier controll points, 0 connect the line, 4 connect the other curve
        vec3f B2[4];

        /**
         * the constructor will init every parameters can known from the three points
         * @param p0 point coming from)
         * @param p1 point at the turnning)
         * @param p2 point leaving to
         */
        TurnPoint_s(const vec3f & p0, const vec3f & p1, const vec3f & p2);

        /// inform that the point has been modified
        void completeSpdModify() {
            spdModifiedComplete = true;
        }

        /// check if the point has been modified
        bool isSpdModifiedComplete() const {
            return spdModifiedComplete;
        }

        /// set the d of this turn, need spdMax and accMax to cal the max speed of this turn
        void setD(double d, double spdMax, double accMax);

        /// using the parameter u to cal the related point
        vec3f calPoint(double u, bool isPartOne) const;

        /**
         * cal the len of one bezier curve(there are two in one turn)
         * @return the len of one curve
         */
        double calPreciseHalfLen();

    private:
        /// record if the speed has modified complete
        bool spdModifiedComplete = false;

    } TurnPoint;
}


#endif //HELLOCPLEX_TURNPOINT_H
