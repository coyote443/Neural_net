#include "header.h"
#include "net.h"
#include "teacher.h"

int main()
{
    double   bias     =  1.000,      //      Bias - dodatkowa waga ze stałym sygnałem = zadanej wartosci || brak
             beta     =  1.100,      //      Wsp. krzywej aktywacji                                      || brak
             eta      =  0.200,      //      Wsp. uczenia
             alfa     =  0.600,      //      Wsp. momentum
             minErr   =  0.05;       //      Błąd poniżej którego nauka jest przerwana
    unsigned blur     =  50;         //      Wsp. Określający dla jakiej rozpietości uśredniać sqErr

    //  CHARAKTERYSTYKA SIECI
    //  Napisz menu z możliwoscia wybrania opcji
    //  Dodaj możliwość pracy z biasem

    vector <double> netChar;
    netChar.push_back(bias);
    netChar.push_back(beta);
    netChar.push_back(eta);
    netChar.push_back(alfa);
    netChar.push_back(blur);
    netChar.push_back(minErr);

    vector<unsigned> topology;
    topology.push_back(35);
    topology.push_back(10);
    topology.push_back(2);


    // Średni błąd - l. zmiennych == wspólczynnikowi rozmycia błędu;
    // Każda ze zmiennych bazowo inicjalizowana jest zerami, następnie co ilość próbek == współczynnikowi rozmycia całość jest sumowana i dzielona przez zero

    Teacher filolog(netChar);

    return 0;
}
