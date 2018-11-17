#include <iostream>
#include <ilcplex/ilocplex.h>
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
    auto a = planner.genCurv({{0,    0,  0},
                              {0.5,  0,  0},
                              {0.5,  -1, 0},
                              {-0.5, -1, 0},
                              {-0.5, 0,  0},
                              {0,    0,  0}}, 0.01, 0.08, 0, 0);
    cout << "complete" << endl;
//    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
//    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
//    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
//    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
//    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
}