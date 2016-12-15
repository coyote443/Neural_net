#include "trans.h"

Trans::Trans(vector<unsigned> &topology, vector<double> &netChar)
{
    loadNetwork();
    networkToLoad = new Net(TOPOLOGY, NETCHAR, WEIGHTS);

    topology = TOPOLOGY;
    netChar  = NETCHAR;

    networkToLoad->drawNetwork(true, true);
}

void Trans::loadNetwork()
{
    fstream Load;
    string takenLine;

    getchar();

    while(true)
    {
        system("cls");
        cout << endl;
        cout << "\tWprowadz nazwe pliku z zapisana siecia " << endl << endl;
        cout << "\tNAZWA PLIKU\t";
        string fileName, fileDir = "Saves/";
        getline(cin, fileName);
        fileDir +=fileName + ".nsave";

        Load.open(fileDir, ios::in);

        if(Load.good() == true)
        {
            break;
        }
        else
        {
            cout << endl;
            cout << "\tZla sciezka / brak dostepu do pliku" << endl;
            getchar();
        }
    }

    getline(Load, takenLine); // Pomijamy wyraz TOPOLOGY
    getline(Load, takenLine);

    // Wczytuje TOPOLOGY
    while(!takenLine.empty())
    {
        unsigned poz = takenLine.find_first_of(";");            // Znajdź pierwszy znak spec
        string subLine = takenLine.substr(0, poz);              // Zrób substring
        takenLine.erase(0, poz+1);                              // Usuń go wraz z ';'
        TOPOLOGY.push_back(stoul(subLine));
    }


    // Wczytuję NETCHAR
    getline(Load, takenLine); // Pomijamy wyraz CHARACTER
    getline(Load, takenLine);

    while(!takenLine.empty())
    {
        unsigned poz = takenLine.find_first_of(";");            // Znajdź pierwszy znak spec
        string subLine = takenLine.substr(0, poz);              // Zrób substring
        takenLine.erase(0, poz+1);                              // Usuń go wraz z ';'
        NETCHAR.push_back(stod(subLine));
    }


    // Wczytuję wagi
    getline(Load, takenLine); // Pomijam wyraz WEIGHTS

    while(!Load.eof())
    {
        getline(Load, takenLine);

        WEIGHTS.push_back(neuron_weights());                         // Robimy wektor dla litery
        while(!takenLine.empty())
        {
            unsigned poz = takenLine.find_first_of(";");         // Znajdź pierwszy znak spec
            string subLine = takenLine.substr(0, poz);           // Zrób substring
            takenLine.erase(0, poz + 1);                         // Usuń go wraz z ';'
            WEIGHTS.back().push_back(stod(subLine));             // Wrzucamy do litery jej dane
        }
    }
    Load.close();
}
