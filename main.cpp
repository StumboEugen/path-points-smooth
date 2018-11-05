#include <iostream>
#include <ilcplex/ilocplex.h>

using namespace std;

int main() {
    IloEnv env;
    IloNumVarArray d;

    IloNumExprArg expr;
    for (int i = 0; i < 10; i++) {
        d.add(IloNumVar(env, 0.0, +IloInfinity));
        expr = expr + d[i];
    }

    IloModel model(env);

    model.add(IloMinimize(env, expr));

    IloRangeArray c(env);
    c.add(expr <= 0);
}