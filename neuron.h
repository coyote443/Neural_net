#ifndef NEURON_H
#define NEURON_H

#include "header.h"
class Neuron;
typedef vector<Neuron> LAYER;

class Neuron
{
public:
    Neuron(unsigned weights_num, double bias);
    Neuron(unsigned weights_num, double bias, vector<double> n_weights);


    void changeOutput(double outNewVal){m_output = outNewVal;}
    void feedForward(LAYER &prevLayer);

    vector<double> & returnWeights(){return m_weights;}
    double returnOutput(){return m_output;}
    void calcOutGrad(double Teach);
    void calcHiddGrad(LAYER & nextLay, unsigned neuron_index);
    void updateWeights(LAYER & prevLay, double eta, double alfa);
private:
    vector<double> m_weights;
    vector<double> m_d_weights;
    double m_gradient;
    double BIAS;
    double m_output = 0;

    double randWeight(void);
    int current_time_nanosec(void);
    double tranFun(double x){return tanh(x);}
    double transferFunctionDerr(double x){return 1.0 - x * x;}

};

#endif // NEURON_H
