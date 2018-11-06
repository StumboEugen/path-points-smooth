//
// Created by stumbo on 18-11-5.
//

#ifndef HELLOCPLEX_TOOLS_H
#define HELLOCPLEX_TOOLS_H

namespace HGJ {

    // 2 (sqrt(6) - 1) / 5
    const static double c1 = 0.579795897113271;
    const static double c2 = (c1 + 4) * (c1 + 1);
    const static double c3 = (c1 + 4) / (c2 + 6);
    const static double c4 = 1 / (1 - c1 * c3 - c3);
    const static double c5 = (c2 + 4) * (c2 + 4) / (54.0 * c3);
}

#endif //HELLOCPLEX_TOOLS_H
