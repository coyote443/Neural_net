#include "header.h"
#include "net.h"
#include "teacher.h"
#include "trans.h"

void drawMenu();
void teachNet(vector<double> &netChar);
void trans(vector<unsigned> topology, vector<double> &netChar);

int main()
{
    drawMenu();
    return 0;
}

void drawMenu()
{
   double   bias     =  1.000,      //      Bias - dodatkowa waga ze stałym sygnałem = zadanej wartosci
            beta     =  1.100,      //      Wsp. krzywej aktywacji                                      || brak
            eta      =  0.0010,     //      Wsp. uczenia
            alfa     =  0.600,      //      Wsp. momentum
            minErr   =  0.001;      //      Błąd poniżej którego nauka jest przerwana

   vector <double> netChar;
   netChar.push_back(bias);
   netChar.push_back(beta);
   netChar.push_back(eta);
   netChar.push_back(alfa);
   netChar.push_back(minErr);

   vector <unsigned> topology;

   bool uit = true;
   while(uit)
   {
       system("cls");
       cout << endl;
       cout << "\t\tMENU" << endl << endl;
       cout << "\t1 - WCZYTAJ SIEC" << endl;
       cout << "\t2 - WYUCZ SIEC" << endl;
       cout << "\t0 - WYJSCIE" << endl;

       unsigned menu;
       cout << endl;
       cout << "\tWPISZ WYBRANA OPCJE ";
       cin >> menu;

       switch(menu)
       {
       case 0:
           uit = false;
           break;

       case 1:
           netChar.clear();
           trans(topology, netChar);
           getchar();
           break;

       case 2:
           teachNet(netChar);
           break;

       default:
           cout << endl;
           cout << "\tNIE MA TAKIEJ OPCJI" << endl;
           getchar();getchar();
           break;
      }
   }
}

void teachNet(vector<double> &netChar)
{
    bool uit = true;

    while(uit)
    {
        system("cls");
        string spec[] = {"BIAS", "BETA", "ETA", "ALFA", "MinERR"};

        cout << endl;
        cout << "\tCHARAKTERYSTYKA SIECI " << endl;
        cout << endl;

        cout << "\t0\tZOSTAW BEZ ZMIAN" << endl << endl;
        for(unsigned draw = 0; draw < netChar.size(); draw++)
        {
            cout << "\t" << draw + 1 << "\t" << spec[draw] << "\t" << netChar[draw] << endl;
        }
        cout << endl;

        cout << "\tWPISZ NR WARTOSCI KTORA CHCESZ ZMODYFIKOWAC ";

        unsigned choose;
        cin >> choose;

        if(choose == 0)
        {
            break;
        }

        else if (choose > 0 && choose <= netChar.size())
        {
            double wyb;
            cout << endl;
            cout << "\tWPROWADZ PORZADANA WARTOSC ";
            cin >> wyb;
            netChar[choose - 1] = wyb;
            cout << endl;
        }
        else if(choose > netChar.size())
        {
            cout << endl;
            cout << "\tPODANO BLEDNA WARTOSC" << endl;
            getchar();getchar();
        }
    }
    getchar();
    Teacher filolog(netChar);

    cout << endl;
    cout << "\tNacisnij dowolny klawisz aby wrocic do menu glownego " << endl;
    getchar();
}

void trans(vector<unsigned> topology, vector<double> &netChar)
{
    Trans test(topology, netChar);
}










