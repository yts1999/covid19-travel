#ifndef SOLVER_H
#define SOLVER_H
#include "args.h"
#include <algorithm>
#include <vector>
#include <queue>


class Solver {
public:
    Solver();
    //风险值，时间，路径
    std::pair<std::pair<double, int>, std::vector<int> > solve(int st, int stTime, int ed);
    std::pair<std::pair<double, int>, std::vector<int> > solve(int st, int stTime, int ed, int timeLimit);
private:
    static const double city_riskval[3];
    static const int transport_riskval[3];
    struct Edge {
        int ed, dur, type, timetable_no;
        Edge() {}
        Edge(int _ed, int _dur, int _type, int _tno) :
            ed(_ed), dur(_dur), type(_type), timetable_no(_tno) {}
    };
    double dis[CITY_NUM][168];
    std::pair<std::pair<int, int>, int> from[CITY_NUM][168];
    bool vis[CITY_NUM][168];
    std::vector<Edge> edges[CITY_NUM][24];
    std::priority_queue<std::pair<int, std::pair<int, int> > , std::vector<std::pair<int, std::pair<int, int> > >, std::greater<std::pair<int, std::pair<int, int> > > > q;
};

#endif // SOLVER_H
