#include "solver.h"
#include <cfloat>

Solver::Solver() {
    for (int i = 0; i < TIMETABLE_SIZE; i++)
        edges[timetable[i].st][timetable[i].stTime].push_back(Edge(timetable[i].ed, timetable[i].dur, timetable[i].type, i));
}

std::pair<std::pair<double, int>, std::vector<int> > Solver::solve(int st, int stTime, int ed) {
    for (int i = 0; i < CITY_NUM; i++)
        for (int j = 0; j < 168; j++) {
            dis[i][j] = DBL_MAX;
            from[i][j] = std::make_pair(std::make_pair(-1, -1), -1);
        }
    memset(vis, 0, sizeof(vis));
    for (; ! q.empty(); q.pop());
    dis[st][stTime] = 0;
    q.push(std::make_pair(0, std::make_pair(st, stTime)));
    for (; ! q.empty();) {
        int x = q.top().second.first, xTime = q.top().second.second;
        q.pop();
        if (vis[x][xTime])
            continue;
        vis[x][xTime] = true;
        for (int i = 0; i < edges[x][xTime % 24].size(); i++)
            if (xTime + edges[x][xTime % 24][i].dur < 168 && dis[x][xTime] + transport_riskval[edges[x][xTime % 24][i].type] * city_riskval[risk_level[x]] * edges[x][xTime % 24][i].dur < dis[edges[x][xTime % 24][i].ed][xTime + edges[x][xTime % 24][i].dur]) {
                dis[edges[x][xTime % 24][i].ed][xTime + edges[x][xTime % 24][i].dur] = dis[x][xTime] + transport_riskval[edges[x][xTime % 24][i].type] * city_riskval[risk_level[x]] * edges[x][xTime % 24][i].dur;
                from[edges[x][xTime % 24][i].ed][xTime + edges[x][xTime % 24][i].dur] = std::make_pair(std::make_pair(x, xTime), edges[x][xTime % 24][i].timetable_no);
                q.push(std::make_pair(dis[edges[x][xTime % 24][i].ed][xTime + edges[x][xTime % 24][i].dur], std::make_pair(edges[x][xTime % 24][i].ed, xTime + edges[x][xTime % 24][i].dur)));
            }
        if (xTime + 1 < 168 && dis[x][xTime] + city_riskval[risk_level[x]] < dis[x][xTime + 1]) {
            dis[x][xTime + 1] = dis[x][xTime] + city_riskval[risk_level[x]];
            from[x][xTime + 1] = std::make_pair(std::make_pair(x, xTime), -1);
            q.push(std::make_pair(dis[x][xTime + 1], std::make_pair(x, xTime + 1)));
        }
    }
    double mn = DBL_MAX;
    int mnTime = -1;
    for (int i = 0; i < 168; i++)
        if (dis[ed][i] < mn) {
            mn = dis[ed][i];
            mnTime = i;
        }
    std::vector<int> path;
    if (mnTime == -1)
        return std::make_pair(std::make_pair(mn, mnTime), path);
    for (int x = ed, xTime = mnTime; x != -1; ) {
        if (from[x][xTime].second != -1)
            path.push_back(from[x][xTime].second);
        int nx = from[x][xTime].first.first;
        xTime = from[x][xTime].first.second;
        x = nx;
    }
    std::reverse(path.begin(), path.end());
    return std::make_pair(std::make_pair(mn, mnTime), path);
}

std::pair<std::pair<double, int>, std::vector<int> > Solver::solve(int st, int stTime, int ed, int timeLimit) {
    timeLimit += stTime;
    for (int i = 0; i < CITY_NUM; i++)
        for (int j = 0; j < 168; j++) {
            dis[i][j] = DBL_MAX;
            from[i][j] = std::make_pair(std::make_pair(-1, -1), -1);
        }
    memset(vis, 0, sizeof(vis));
    for (; ! q.empty(); q.pop());
    dis[st][stTime] = 0;
    q.push(std::make_pair(0, std::make_pair(st, stTime)));
    for (; ! q.empty();) {
        int x = q.top().second.first, xTime = q.top().second.second;
        q.pop();
        if (vis[x][xTime])
            continue;
        vis[x][xTime] = true;
        for (int i = 0; i < edges[x][xTime % 24].size(); i++)
            if (xTime + edges[x][xTime % 24][i].dur <= timeLimit && dis[x][xTime] + transport_riskval[edges[x][xTime % 24][i].type] * city_riskval[risk_level[x]] * edges[x][xTime % 24][i].dur < dis[edges[x][xTime % 24][i].ed][xTime + edges[x][xTime % 24][i].dur]) {
                dis[edges[x][xTime % 24][i].ed][xTime + edges[x][xTime % 24][i].dur] = dis[x][xTime] + transport_riskval[edges[x][xTime % 24][i].type] * city_riskval[risk_level[x]] * edges[x][xTime % 24][i].dur;
                from[edges[x][xTime % 24][i].ed][xTime + edges[x][xTime % 24][i].dur] = std::make_pair(std::make_pair(x, xTime), edges[x][xTime % 24][i].timetable_no);
                q.push(std::make_pair(dis[edges[x][xTime % 24][i].ed][xTime + edges[x][xTime % 24][i].dur], std::make_pair(edges[x][xTime % 24][i].ed, xTime + edges[x][xTime % 24][i].dur)));
            }
        if (xTime + 1 <= timeLimit && dis[x][xTime] + city_riskval[risk_level[x]] < dis[x][xTime + 1]) {
            dis[x][xTime + 1] = dis[x][xTime] + city_riskval[risk_level[x]];
            from[x][xTime + 1] = std::make_pair(std::make_pair(x, xTime), -1);
            q.push(std::make_pair(dis[x][xTime + 1], std::make_pair(x, xTime + 1)));
        }
    }
    double mn = DBL_MAX;
    int mnTime = -1;
    for (int i = 0; i <= timeLimit; i++)
        if (dis[ed][i] < mn) {
            mn = dis[ed][i];
            mnTime = i;
        }
    std::vector<int> path;
    if (mnTime == -1)
        return std::make_pair(std::make_pair(mn, mnTime), path);
    for (int x = ed, xTime = mnTime; x != -1; ) {
        if (from[x][xTime].second != -1)
            path.push_back(from[x][xTime].second);
        int nx = from[x][xTime].first.first;
        xTime = from[x][xTime].first.second;
        x = nx;
    }
    std::reverse(path.begin(), path.end());
    return std::make_pair(std::make_pair(mn, mnTime), path);
}
