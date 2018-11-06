#include <iostream>
#include <ilcplex/ilocplex.h>
#include "HGJPathSmooth/pathSmoother.h"

using namespace std;

int main() {

    HGJ::pathPlanner pp;

    try {
        IloEnv env;
        IloNumVarArray d(env);

        IloNumExprArg expr;
        for (int i = 0; i < 10; i++) {
            d.add(IloNumVar(env, 0.0, +IloInfinity));
        }


        IloModel model(env);

        model.add(IloMinimize(env, expr));

        IloRangeArray c(env);
        c.add(expr <= 0);

        double a = 0.579795897113271 * 5.0 / 2.0 + 1;
        cout << a * a;
    } catch (IloException &e) {
        cout << e << endl;
    }
}