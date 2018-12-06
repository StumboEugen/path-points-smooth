#include <iostream>
#include <ilcplex/ilocplex.h>
#include <fstream>

#include "HGJPathSmooth/pathSmoother.h"

using namespace std;

int main() {

    HGJ::pathPlanner planner;
    HGJ::WayPoints wayPoints;

//    planner.genCurv({{0,0,0},
//                     {0.3,0,0},
//                     {0.5,-0.6,0},
//                     {-0.5,-1,0},
//                     {-0.4,0,0},
//                     {0,0,0}}, 0.01, 0.08);

//    auto a = planner.genCurv({{0,    0,  0},
//                              {0.5,  0,  0},
//                              {0.5,  -1, 0},
//                              {-0.5, -1, 0},
//                              {-0.5, 0,  0},
//                              {0,    0,  0}}, 0.01, 0.08, 0, 0);

    auto a = planner.genCurv({{0,    0,  0},
                              {0.4,  0,  0},
                              {0.4,  -0.1,  0},
                              {0.6,  -0.1,  0},
                              {1.0,  -0.2,  0},
                              {0.5,  -1, 0},
                              {-0.5, -1, 0},
                              {-0.5, 0,  0},
                              {0,    0,  0}}, 0.01, 0.08, 0, 0);
    cout << "complete" << endl;

    fstream fs;
    fs.open("curv", ios::out);
    for (const auto & point: a) {
        fs << point << endl;
    }
//    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
//    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
//    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
//    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
//    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
}