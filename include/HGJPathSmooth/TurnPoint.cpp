//
// Created by stumbo on 18-11-5.
//

#include "TurnPoint.h"

HGJ::TurnPoint_s::TurnPoint_s(const HGJ::vec3f & p0, const HGJ::vec3f & p1,
                              const HGJ::vec3f & p2)
        : t1(p1 - p0 / (p1 - p0).len()),
          t2(p2 - p1 / (p2 - p1).len())
{

}
