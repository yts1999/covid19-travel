#ifndef ARGS_H
#define ARGS_H
#include <QString>

#define BEIJING       0
#define TIANJIN       1
#define SHIJIAZHUANG  2
#define TAIYUAN       3
#define HOHHOT        4
#define SHENYANG      5
#define DALIAN        6
#define CHANGCHUN     7
#define HARBIN        8
#define SHANGHAI      9
#define NANJING       10
#define HANGZHOU      11
#define NINGBO        12
#define HEFEI         13
#define FUZHOU        14
#define XIAMEN        15
#define NANCHANG      16
#define JINAN         17
#define QINGDAO       18
#define ZHENGZHOU     19
#define WUHAN         20
#define CHANGSHA      21
#define GUANGZHOU     22
#define SHENZHEN      23
#define NANNING       24
#define HAIKOU        25
#define CHENGDU       26
#define GUIYANG       27
#define KUNMING       28
#define LHASA         29
#define CHONGQING     30
#define XIAN          31
#define LANZHOU       32
#define XINING        33
#define YINCHUAN      34
#define URUMQI        35
#define TAIPEI        36

#define TRAIN    0
#define AIRPLANE 1
#define COACH    2

#define CITY_NUM 37
extern const QString cities[37];
extern const int risk_level[37];

struct Timetable {
    int st, ed, stTime, dur, type;
};

#define TIMETABLE_SIZE 322
extern const Timetable timetable[350];

#endif // ARGS_H
