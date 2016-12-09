#include "header.h"
#include "net.h"
#include "teacher.h"


void teacher(Net toTeach);

void teach();

int main()
{
    double  bias     =  1.000,      //      Bias - dodatkowa waga ze stałym sygnałem = zadanej wartosci
            beta     =  1.100,      //      Wsp. krzywej aktywacji || brak
            eta      =  0.200,      //      Wsp. uczenia
            alfa     =  0.600,      //      Wsp. momentum
            blur     =  20,         //      Wsp. Określający w jakim zakresie uśredniać sqErr
            minErr   =  0.005;      //      Błąd poniżej którego nauka jest przerwana

    //  CHARAKTERYSTYKA SIECI XOR

    vector <double> netChar;
    netChar.push_back(bias);
    netChar.push_back(beta);
    netChar.push_back(eta);
    netChar.push_back(alfa);
    netChar.push_back(blur);
    netChar.push_back(minErr);

    vector<unsigned> topology;
    topology.push_back(2);
    topology.push_back(4);
    topology.push_back(1);


    // Średni błąd - l. zmiennych == wspólczynnikowi rozmycia błędu;
    // Każda ze zmiennych bazowo inicjalizowana jest zerami, następnie co ilość próbek == współczynnikowi rozmycia całość jest sumowana i dzielona przez zero

    Net Siec(topology, netChar);
    //teacher(Siec);
    //Teacher filolog(netChar);

    Siec.saveNetwork();
    cout << "lalala";

    return 0;
}


void teacher(Net toTeach)
{
    vector<double> S1;
    S1.push_back(0.0);
    S1.push_back(0.0);
    vector<double> T1;
    T1.push_back(0.0);

    vector<double> S2;
    S2.push_back(1.0);
    S2.push_back(0.0);
    vector<double> T2;
    T2.push_back(1.0);

    vector<double> S3;
    S3.push_back(0.0);
    S3.push_back(1.0);
    vector<double> T3;
    T3.push_back(1.0);

    vector<double> S4;
    S4.push_back(1.0);
    S4.push_back(1.0);
    vector<double> T4;
    T4.push_back(0.0);

    int epoch = 0;
    bool flag = true;
    while(flag)
    {
        epoch++;
        int randtaker = rand()%4;

            if(randtaker == 0)
            {
                toTeach.feedForward(S1);
                if(!toTeach.backProp(T1))
                    flag = false;
            }

            if(randtaker == 1)
            {
                toTeach.feedForward(S2);
                if(!toTeach.backProp(T2))
                    flag = false;
            }

            if(randtaker == 2)
            {
                toTeach.feedForward(S3);
                if(!toTeach.backProp(T3))
                    flag = false;
            }

            if(randtaker == 3)
            {
                toTeach.feedForward(S4);
                if(!toTeach.backProp(T4))
                    flag = false;
            }
    }

    toTeach.feedForward(S1);
    toTeach.drawNetwork(false,true);

    toTeach.feedForward(S2);
    toTeach.drawNetwork(false,true);

    toTeach.feedForward(S3);
    toTeach.drawNetwork(false,true);

    toTeach.feedForward(S4);
    toTeach.drawNetwork(false,true);


}
