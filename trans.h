#ifndef TRANS_H
#define TRANS_H


#include "net.h"

typedef vector<double> neuron_weights;

class Trans
{
public:
    Trans(vector<unsigned> &topology, vector<double> &netChar);
    void loadNetwork();

private:
    Net * networkToLoad;

    vector<unsigned> TOPOLOGY;
    vector<double>   NETCHAR;
    vector<neuron_weights>WEIGHTS;
};

#endif // TRANS_H
