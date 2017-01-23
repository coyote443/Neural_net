#ifndef NET_H
#define NET_H

#include "header.h"
#include "neuron.h"

typedef vector<double> NET_WEIGHTS;
typedef vector<NET_WEIGHTS> NET_LAYER;

class Net
{
public:
    Net(vector<unsigned> &topology, vector<double> &netChar);
    Net(vector<unsigned> &topology, vector<double> &netChar, vector<NET_LAYER> &weights);

    void feedForward (vector<double> & inputSig);
    bool backProp(vector<double> & teachSig, unsigned uSigSize);
    double validator(vector<double> &valSig, unsigned uSigSize);
    vector<double> getOutput(bool drawOutput = false);

    void drawNetwork(bool weights, bool signalStrength);
    void saveNetwork(double errRate, string fileName);

private:
    vector<NEURON_LAYER> NETWORK;
    vector<NET_LAYER> WEIGHTS;
    vector<unsigned> TOPOLOGY;
    vector<double> NETCHAR;
    double BIAS, BETA, ETA, ALFA, MIN_ERR;

    long double sqErrBlur = 0;
    long double sqErrVal  = 0;

    bool sqErrBlurFun(double sqErr);
};

#endif // NET_H
