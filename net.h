#ifndef NET_H
#define NET_H

#include "header.h"
#include "neuron.h"

typedef vector<double> neuron_weights;

class Net
{
public:
    Net(vector<unsigned> &topology, vector<double> &netChar);
    Net(vector<unsigned> &topology, vector<double> &netChar, vector<neuron_weights> &weights);

    void feedForward (vector<double> & inputSig);
    bool backProp(vector<double> & teachSig);
    vector<double> getOutput(bool drawOutput = false);

    void drawNetwork(bool weights, bool signalStrength);
    void saveNetwork();

private:
    vector<LAYER> NETWORK;
    vector<neuron_weights> WEIGHTS;
    vector<unsigned> TOPOLOGY;
    vector<double> NETCHAR;
    double BIAS, BETA, ETA, ALFA, BLUR, MIN_ERR;

    double sqErrBlur = 0;
    bool sqErrBlurFun(double sqErr);
};

#endif // NET_H
