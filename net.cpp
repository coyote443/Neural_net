#include "net.h"

Net::Net(vector<unsigned> &topology, vector<double> &netChar)
{
    TOPOLOGY = topology;
    NETCHAR  = netChar;

    BIAS = netChar[0];
    BETA = netChar[1];
    ETA  = netChar[2];
    ALFA = netChar[3];
    MIN_ERR = netChar[4];

    // Do określenia wielkosci sieci
    double allNeurons = 0, createdNeuronsCounter = 0;

    for(unsigned L = 0; L < TOPOLOGY.size(); L++)
    {
        allNeurons += TOPOLOGY[L];
    }


    for(unsigned L = 0; L < TOPOLOGY.size(); L++)
    {
        NETWORK.push_back(NEURON_LAYER());
        for(unsigned N = 0; N < TOPOLOGY[L]; N++)
        {
            system("cls");
            cout << endl;
            cout << "\tZbudowano obecnie " << createdNeuronsCounter / allNeurons * 100 << "% sieci" << endl << endl;
            if(L > 0)
            {
                NETWORK[L].push_back( Neuron(TOPOLOGY[L - 1], BIAS) );
                createdNeuronsCounter++;
            }
            else
            {
                NETWORK[L].push_back( Neuron(0, BIAS) );
                createdNeuronsCounter++;
            }
        }
    }
}

Net::Net(vector<unsigned> &topology, vector<double> &netChar, vector<NET_LAYER> &weights)
{
    TOPOLOGY = topology;
    NETCHAR  = netChar;
    WEIGHTS  = weights;

    BIAS = netChar[0];
    BETA = netChar[1];
    ETA  = netChar[2];
    ALFA = netChar[3];
    MIN_ERR = netChar[4];

    // Do określenia wielkosci sieci
    double allNeurons = 0, createdNeuronsCounter = 0;

    for(unsigned L = 0; L < TOPOLOGY.size(); L++)
    {
        allNeurons += TOPOLOGY[L];
    }


    for(unsigned L = 0; L < TOPOLOGY.size(); L++)
    {
        NETWORK.push_back(NEURON_LAYER());
        for(unsigned N = 0; N < TOPOLOGY[L]; N++)
        {
            system("cls");
            cout << endl;
            cout << "\tZbudowano obecnie " << createdNeuronsCounter / allNeurons * 100 << "% sieci" << endl << endl;
            if(L > 0)
            {
                NETWORK[L].push_back( Neuron(TOPOLOGY[L - 1], BIAS, WEIGHTS[L][N]));
                createdNeuronsCounter++;
            }
            else
            {
                NETWORK[L].push_back( Neuron(0, BIAS) );
                createdNeuronsCounter++;
            }
        }
    }
}

void Net::feedForward (vector<double> &inputSig)
{
    for(unsigned I = 0; I < inputSig.size(); I++)
    {
        NETWORK[0][I].changeOutput(inputSig[I]);
    }

    for(unsigned L = 1; L < TOPOLOGY.size(); L++)
    {
        NEURON_LAYER &prevLayer = NETWORK[L - 1];
        for(unsigned N = 0; N < TOPOLOGY[L]; N++)
        {
            NETWORK[L][N].feedForward(prevLayer);
        }
    }
}

void Net::drawNetwork(bool weights, bool signalStrength)
{
    cout << fixed << endl;
    if (weights == true)
    {
        cout << "WEIGHTS";
        for(unsigned L = 0; L < TOPOLOGY.size(); L++)
        {
            for(unsigned N = 0; N < TOPOLOGY[L]; N++)
            {
                cout << endl << "N " << N + 1 << "    ";
                unsigned Weight = NETWORK[L][N].returnWeights().size();
                for(unsigned W = 0; W < Weight; W++)
                {
                    if(NETWORK[L][N].returnWeights()[W] >= 0)
                        cout << " ";
                    cout << NETWORK[L][N].returnWeights()[W];
                    cout << "   ";
                }
            }
            cout << endl;
        }
        cout << endl << endl << endl;
    }

    if(signalStrength == true)
    {
        cout << "SIGNAL STRENGTH" << endl << endl;
        for(unsigned L = 0; L < TOPOLOGY.size(); L++)
        {
            for(unsigned N = 0; N < TOPOLOGY[L]; N++)
            {
                if(NETWORK[L][N].returnOutput() >= 0)
                    cout << " ";
                cout << NETWORK[L][N].returnOutput() << endl;
            }
            cout << endl;
        }
    }
    cout.precision(5);
}

bool Net::backProp(vector<double> &teachSig, unsigned uSigSize)
{
    // Liczę błąd średniokwadratowy
    double sqErr = 0;
    NEURON_LAYER &OUTLAYER = NETWORK.back();

    for(unsigned E = 0; E < teachSig.size(); E++)
    {
        sqErr += (teachSig[E] - OUTLAYER[E].returnOutput()) * (teachSig[E] - OUTLAYER[E].returnOutput());
    }
    sqErr /= teachSig.size();
    sqErr = sqrt(sqErr);

    // Czasem sqErr = 0; pomijam te wyniki z zerem usredniam Err
    static int sqErrCounter = uSigSize;
    if(sqErr > 0)
    {
        sqErrCounter--;

        sqErrBlur += sqErr;

        if(sqErrCounter == 0)
        {
            sqErrBlur /= uSigSize;
            long double tmp_err = sqErrBlur;
            cout << "\tProgress  =\t" << MIN_ERR / sqErrBlur * 100  <<"%" << endl;
            cout << "\tsqErr     =\t" << sqErrBlur << endl;

            sqErrBlur = 0, sqErrCounter = uSigSize;

            if(tmp_err < MIN_ERR)
                return false;
        }
    }

    // Liczę gradient na outpucie
    for(unsigned N = 0; N < teachSig.size(); N++)
    {
        OUTLAYER[N].calcOutGrad(teachSig[N]);
    }


    // Liczę gradient we wszystkich warstwach hidden
    for(int L = NETWORK.size() - 2 ; L >= 0 ; L--)          // czy włączyć input? // właczony
    {
        NEURON_LAYER &nextLay = NETWORK[L + 1];
        NEURON_LAYER &acctLay = NETWORK[L];

        for(unsigned N = 0; N < acctLay.size(); N++)
        {
            acctLay[N].calcHiddGrad(nextLay,N);
        }
    }


    // Zmieniam wartość wag
    for (unsigned layerN = NETWORK.size() - 1; layerN > 0; layerN--)
    {
        NEURON_LAYER &prevLayer = NETWORK[layerN - 1];
        NEURON_LAYER &acctLayer = NETWORK[layerN];

        for(unsigned N = 0; N < NETWORK[layerN].size(); N++)
        {
            acctLayer[N].updateWeights(prevLayer, ETA, ALFA);
        }
    }
    return true;
}

double Net::validator(vector<double> &valSig, unsigned uSigSize)
{
    // Liczę błąd średniokwadratowy
    double sqErr = 0;

    NEURON_LAYER &OUTLAYER = NETWORK.back();

    for(unsigned E = 0; E < valSig.size(); E++)
    {
        sqErr += (valSig[E] - OUTLAYER[E].returnOutput()) * (valSig[E] - OUTLAYER[E].returnOutput());
    }

    // Czasem sqErr = 0; pomijam te wyniki z zerem usredniam Err
    static int sqErrCounter = uSigSize;
    if(sqErr > 0)
    {
        sqErrCounter--;

        sqErrVal += sqErr;

        if(sqErrCounter == 0)
        {
            sqErrVal /= uSigSize;
            double toOut = sqErrVal;

            cout << endl;
            cout << "\tValidation=\t" << sqErrVal << endl;

            sqErrVal = 0, sqErrCounter = uSigSize;
            return toOut;
        }
    }
    return 0;
}

void Net::saveNetwork(double errRate, string fileName)
{
    fstream networkState;
    string dir = "Saves/" + fileName + ".nsave";

    networkState.open(dir, ios::out | ios::trunc);

    networkState << "TOPOLOGY" << "\t\t[ErrRate " << errRate << " ]" << endl;
    for(unsigned t = 0; t < TOPOLOGY.size(); t++)
        networkState << TOPOLOGY[t] << ";";
    networkState << endl;

    networkState << "CHARACTER" << endl;
    for(unsigned t = 0; t < NETCHAR.size(); t++)
        networkState << NETCHAR[t] << ";";
    networkState << endl;

    networkState << "WEIGHTS" << endl;
    for(unsigned L = 0; L < TOPOLOGY.size(); L++)
    {
        if(L > 0)
            networkState << endl;
        for(unsigned N = 0; N < TOPOLOGY[L]; N++)
        {
            if(N > 0)
                networkState << endl;
            unsigned Weight = NETWORK[L][N].returnWeights().size();
            for(unsigned W = 0; W < Weight; W++)
            {
                networkState << NETWORK[L][N].returnWeights()[W];
                networkState << ";";
            }
        }
    }
    networkState.close();
    cout << endl;
    cout << "\tSiec jest zapisana jako " << dir << endl;
}

vector<double> Net::getOutput(bool drawOutput)
{
    vector<double> Res;

    for(unsigned r = 0; r < (TOPOLOGY.back()); r++)
    {
        Res.push_back(NETWORK[TOPOLOGY.size() - 1][r].returnOutput());          // Odwołuje się do ostatniej warstwy
        if(drawOutput == true)
        {
            cout << NETWORK[TOPOLOGY.size() - 1][r].returnOutput() << "\t";
        }
    }

    return Res;
}
