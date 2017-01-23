#ifndef TEACHER_H
#define TEACHER_H

#include "net.h"
#include "header.h"

typedef vector<double> LETTER;

class Teacher
{
public:
    Teacher(vector<double> &netChar);                                       // Do szkolenia sieci
private:
    Net * networkToTeach;

    string FILENAME;            // Posłuży jako nazwa do zapisu
    unsigned EPOCH;

    vector<LETTER> SIGNALS;
    vector<LETTER> RESPONSES;

    vector<LETTER> SIGNALS_VAL;
    vector<LETTER> RESPONSES_VAL;

    vector<unsigned> TOPOLOGY;
    vector<double>   NETCHAR;

    void createRespAndTopo();
    void createValidationSet();
    void teachNetwork();
    void drawResults();
};

#endif // TEACHER_H
