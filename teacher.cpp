#include "teacher.h"

Teacher::Teacher(vector<double> &netChar)
{
    NETCHAR = netChar;
    createRespAndTopo();
    networkToTeach = new Net( TOPOLOGY, NETCHAR );
    teachNetwork();
    drawResults();
    cout << endl;
    networkToTeach->saveNetwork( NETCHAR[5], FILENAME );    // Wysyłam ERRRATE
}

void Teacher::createRespAndTopo()
{
    fstream toTeach;
    string takenLine;

    while(true)
    {
        system("cls");
        cout << endl;
        cout << "\tWprowadz nazwe pliku z danymi wejsciowymi z folderu Signals " << endl << endl;
        cout << "\tNAZWA PLIKU\t";
        string fileName, fileDir = "Signals/";
        getline(cin, fileName);
        fileDir +=fileName + ".nsignal";

        FILENAME = fileName;

        toTeach.open(fileDir, ios::in);

        if(toTeach.good() == true)
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

    getline(toTeach, takenLine); // Pomijamy wyraz TOPOLOGY

    getline(toTeach, takenLine);

    // Wczytuje TOPOLOGY
    while(!takenLine.empty())
    {
        unsigned poz = takenLine.find_first_of(";");            // Znajdź pierwszy znak spec
        string subLine = takenLine.substr(0, poz);              // Zrób substring
        takenLine.erase(0, poz+1);                              // Usuń go wraz z ';'
        TOPOLOGY.push_back(stoul(subLine));
    }

    getline(toTeach, takenLine);

    // Wczytuję SIGNALS
    unsigned lineCounter = 3;
    while(!toTeach.eof())
    {
        getline(toTeach, takenLine);

        if(takenLine == "RESPONSES")
        {
            break;
        }

        SIGNALS.push_back(LETTER());                         // Robimy wektor dla litery
        while(!takenLine.empty())
        {
            unsigned poz = takenLine.find_first_of(";");         // Znajdź pierwszy znak spec
            string subLine = takenLine.substr(0, poz);           // Zrób substring
            takenLine.erase(0, poz + 1);                         // Usuń go wraz z ';'
            SIGNALS.back().push_back(stod(subLine));             // Wrzucamy do litery jej dane
        }

        if(SIGNALS.back().size() != TOPOLOGY[0])
        {
            cout << endl;
            cout << "\tStruktura danych a ilosc neuronow wejsciowych - NIEZGODNE\nLINIA: " << lineCounter << endl;
            getchar();
        }

        lineCounter++;
    }
    if(SIGNALS.size() != TOPOLOGY.back())
    {
        cout << endl;
        cout << "\tIlosc sygnalow wejsciowych nie jest zgodna z liczba wyjsc sieci" << endl;
        getchar();
    }


    // Ładujemy wartości dla RESPONSES
    while(!toTeach.eof())
    {
        getline(toTeach, takenLine);

        RESPONSES.push_back(LETTER());
        while(!takenLine.empty())
        {
            unsigned poz = takenLine.find_first_of(";");         // Znajdź pierwszy znak spec
            string subLine = takenLine.substr(0, poz);           // Zrób substring
            takenLine.erase(0, poz + 1);                         // Usuń go wraz z ';'
            RESPONSES.back().push_back(stod(subLine));           // Wrzucamy do litery jej dane
        }
    }
    if(SIGNALS.size() != TOPOLOGY.back())                           // CZY TA LINIJKA MA SENS <?>
    {
        cout << endl;
        cout << "\tIlosc sygnalow uczacych nie jest zgodna z liczba wyjsc sieci" << endl;
        getchar();
    }

    toTeach.close();
}

void Teacher::teachNetwork()
{
    int epoch = 0;

    while(true)
    {
        bool flag = true;
        while(flag)
        {
            epoch++;
            int randS = rand()%TOPOLOGY.back();

            networkToTeach->feedForward(SIGNALS[randS]);

            if(networkToTeach->backProp(RESPONSES[randS]) == false)
                flag = false;
        }


        // Jeśli sieć przeszła przez ciąg uczący, to upewniamy sie czy nie zostały jakieś nierozróżnialne klasy sygnałów
        vector<unsigned> unTeachSignals;

        for(unsigned Sig = 0; Sig < SIGNALS.size(); Sig++)
        {
            networkToTeach->feedForward(SIGNALS[Sig]);

            vector<double> checkThisOutput = networkToTeach->getOutput(false);

            for(unsigned S = 0; S < checkThisOutput.size(); S++)
            {
                if(checkThisOutput[S] > 0.3 && checkThisOutput[S] < 0.7)
                    unTeachSignals.push_back(Sig);
            }
        }

        if(unTeachSignals.size() == 0)          // Jeśli brak - kończymy
            break;

        for(unsigned x = 0; x < 50; x++)       // Jeśli zostały - staramy się nauczyć sieć ich rozróżniania
        {
            epoch++;
            int randS = rand() % unTeachSignals.size();

            networkToTeach->feedForward(SIGNALS[unTeachSignals[randS]]);
            networkToTeach->backProp(RESPONSES[unTeachSignals[randS]]);
        }
    }

    cout << endl;
    cout << "\tSiec nauczyla sie wzorca po " << epoch << " epokach" << endl;
}

void Teacher::drawResults()
{
    typedef vector<double> RESULT;
    vector<RESULT> OutputsArr;

    //Koloruje wyniki
    HANDLE hOut;
    hOut = GetStdHandle( STD_OUTPUT_HANDLE );

    for(unsigned Sig = 0; Sig < TOPOLOGY.back(); Sig++)
    {
        networkToTeach->feedForward( SIGNALS[ Sig ] );
        OutputsArr.push_back( networkToTeach->getOutput( false ) );
    }

    cout << fixed << endl;
    cout.precision(3);

    cout << "Signal\t\t";
    for(unsigned x = 0; x < TOPOLOGY.back(); x++)
    {
        if( (x + 1) > 8)
            cout << x + 1 << "     ";
        else
        {
            cout << x + 1 << "      ";
        }
    }

    cout << endl;

    for(unsigned line = 0; line < TOPOLOGY.back(); line++)
    {
        cout << "\t" << line + 1 << " ";
        if((line + 1) < 10)
            cout << " ";
        for(unsigned column = 0; column < OutputsArr[line].size(); column++)
        {
            if(OutputsArr[line][column] < 0.2)
                SetConsoleTextAttribute( hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED );

            else if(OutputsArr[line][column] >= 0.9)
                SetConsoleTextAttribute( hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);

            else if(OutputsArr[line][column] >= 0.7 && OutputsArr[line][column] < 0.9)
                SetConsoleTextAttribute( hOut, FOREGROUND_RED );

            else if(OutputsArr[line][column] >= 0.3 && OutputsArr[line][column] < 0.7)
                SetConsoleTextAttribute( hOut, FOREGROUND_BLUE | FOREGROUND_RED);

            if(OutputsArr[column][line] >= 0)
                cout << " ";

            cout << OutputsArr[column][line] << " ";

            SetConsoleTextAttribute( hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED );
        }
        cout << endl;
    }
}
