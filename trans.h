#ifndef TRANS_H
#define TRANS_H

#include "net.h"

typedef vector<double> T_WEIGHTS;
typedef vector<T_WEIGHTS> T_LAYER;

class Trans
{
public:
    Trans(vector<unsigned> &topology, vector<double> &netChar);
    void loadNetwork();
private:
    Net * networkToLoad;

    vector<unsigned> TOPOLOGY;
    vector<double>   NETCHAR;
    vector<T_LAYER>WEIGHTS;
};

#endif // TRANS_H
