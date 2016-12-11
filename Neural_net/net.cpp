#include "net.h"

Net::Net(vector<unsigned> &topology, vector<double> &netChar)
{
    TOPOLOGY = topology;
    NETCHAR  = netChar;

    BIAS = netChar[0];
    BETA = netChar[1];
    ETA  = netChar[2];
    ALFA = netChar[3];
    BLUR = netChar[4];
    MIN_ERR = netChar[5];

    // Do określenia wielkosci sieci
    double allNeurons = 0, createdNeuronsCounter = 0;

    for(unsigned L = 0; L < TOPOLOGY.size(); L++)
    {
        allNeurons += TOPOLOGY[L];
    }


    for(unsigned L = 0; L < TOPOLOGY.size(); L++)
    {
        NETWORK.push_back(LAYER());
        for(unsigned N = 0; N < TOPOLOGY[L]; N++)
        {
            system("cls");
            cout << endl;
            cout << "\tZbudowano obecnie " << createdNeuronsCounter / allNeurons * 100 << "% sieci" << endl << endl;
            if(L > 0)
            {
                NETWORK[L].push_back( Neuron(TOPOLOGY[L - 1], BIAS));
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
        LAYER &prevLayer = NETWORK[L - 1];
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

bool Net::backProp(vector<double> &teachSig)
{

    // Liczę błąd średniokwadratowy
    double sqErr = 0;
    LAYER &OUTLAYER = NETWORK.back();

    for(unsigned E = 0; E < teachSig.size(); E++)
        sqErr += (teachSig[E] - OUTLAYER[E].returnOutput()) * (teachSig[E] - OUTLAYER[E].returnOutput());
    sqErr /= teachSig.size();
    sqErr = sqrt(sqErr);



    // Czasem sqErr = 0; pomijam te wyniki z zerem usredniam Err, zgodnie ze wsp. BLUR
    static int sqErrCounter = BLUR;
    if(sqErr != 0)
    {
        sqErrCounter--;

        sqErrBlur += sqErr;

        if(sqErrCounter == 0)
        {
            sqErrBlur /= BLUR;
            double tmp_err = sqErrBlur;

            static int slowDown = 20;
            slowDown --;

            if(slowDown == 0)
            {
                system("cls");
                cout << endl;
                cout << "\tProgress  =\t" << MIN_ERR / sqErrBlur * 100  <<"%" << endl;
                slowDown = 20;
                cout << "\tsqErrBlur =\t" << sqErrBlur << endl;
            }

            sqErrBlur = 0, sqErrCounter = BLUR;

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
        LAYER &nextLay = NETWORK[L + 1];
        LAYER &acctLay = NETWORK[L];

        for(unsigned N = 0; N < acctLay.size(); N++)
        {
            acctLay[N].calcHiddGrad(nextLay,N);
        }
    }


    // Zmieniam wartość wag
    for (unsigned layerN = NETWORK.size() - 1; layerN > 0; layerN--)
    {
        LAYER &prevLayer = NETWORK[layerN - 1];
        LAYER &acctLayer = NETWORK[layerN];

        for(unsigned N = 0; N < NETWORK[layerN].size(); N++)
        {
            acctLayer[N].updateWeights(prevLayer, ETA, ALFA);
        }
    }
    return true;
}

void Net::saveNetwork()
{
    fstream networkState;
    networkState.open("Saves/networkSTATE.nsave", ios::out | ios::trunc);

    networkState << "TOPOLOGY" << endl;
    for(unsigned t = 0; t < TOPOLOGY.size(); t++)
        networkState << TOPOLOGY[t] << "\t";
    networkState << endl << endl;

    networkState << "BIAS\tBETA\tETA \tALFA" << endl;
    for(unsigned t = 0; t < NETCHAR.size(); t++)
        networkState << NETCHAR[t] << "\t\t";
    networkState << endl << endl;

    networkState << "WEIGHTS" << endl;
    for(unsigned L = 0; L < TOPOLOGY.size(); L++)
    {
        for(unsigned N = 0; N < TOPOLOGY[L]; N++)
        {
            unsigned Weight = NETWORK[L][N].returnWeights().size();
            for(unsigned W = 0; W < Weight; W++)
            {
                networkState << NETWORK[L][N].returnWeights()[W];
                networkState << "   ";
            }
        }
        networkState << endl;
    }
    networkState.close();
    cout << endl;
    cout << "\tSiec jest zapisana jako 'Saves/networkSTATE.nsave'" << endl;
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
