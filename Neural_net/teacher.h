#ifndef TEACHER_H
#define TEACHER_H

#include "net.h"

typedef vector<double> LETTER;

class Teacher
{
public:
    Teacher(vector<double> &netChar);
private:
    Net * networkToTeach;

    vector<LETTER> SIGNALS;
    vector<LETTER> RESPONSES;
    vector<unsigned> TOPOLOGY;
    vector<double>   NETCHAR;

    void createRespAndTopo();
    void teachNetwork();
    void drawResults();
};

#endif // TEACHER_H
