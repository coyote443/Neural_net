#include "teacher.h"

Teacher::Teacher(vector<double> &netChar)
{
    NETCHAR = netChar;

    createRespAndTopo();
    createValidationSet();

    networkToTeach = new Net( TOPOLOGY, NETCHAR );

    teachNetwork();
    drawResults();
    cout << endl;
    networkToTeach->saveNetwork( NETCHAR[5], FILENAME );    // Wysyłam ERRRATE
}

void Teacher::createRespAndTopo()
{
    fstream sigFile;
    string takenLine;

    while(true)
    {
        system("cls");
        cout << endl;
        cout << "\tWprowadz nazwe pliku z danymi wejsciowymi z folderu Signals " << endl << endl;
        cout << "\tNAZWA PLIKU\t";
        string fileName, signalDir = "Signals/";
        getline( cin, fileName );
        signalDir += fileName + ".nsignal";

        FILENAME = fileName;

        sigFile.open( signalDir, ios::in );

        if(sigFile.good() == true)
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

    getline( sigFile, takenLine ); // Pomijamy wyraz TOPOLOGY

    getline( sigFile, takenLine );

    // Wczytuje TOPOLOGY
    while( ! takenLine.empty() )
    {
        unsigned poz = takenLine.find_first_of( ";" );            // Znajdź pierwszy znak spec
        string subLine = takenLine.substr( 0, poz );              // Zrób substring
        takenLine.erase( 0, poz + 1 );                            // Usuń go wraz z ';'
        TOPOLOGY.push_back( stoul(subLine) );
    }

    getline( sigFile, takenLine );

    // Wczytuję SYGNAŁ I ODPOWIEDZI
    while( ! sigFile.eof() )
    {
        getline(sigFile, takenLine);

        SIGNALS.push_back( LETTER() );                         	 // Robimy wektor dla litery
        while( ! takenLine.empty() )
        {
            unsigned poz = takenLine.find_first_of( ";" );         // Znajdź pierwszy znak spec
            string subLine = takenLine.substr( 0, poz );           // Zrób substring
            takenLine.erase( 0, poz + 1 );                         // Usuń go wraz z ';'
            SIGNALS.back().push_back( stod(subLine) );             // Wrzucamy do litery jej dane
        }

        getline(sigFile, takenLine);

        RESPONSES.push_back(LETTER());
        while( ! takenLine.empty() )
        {
            unsigned poz = takenLine.find_first_of( ";" );         // Znajdź pierwszy znak spec
            string subLine = takenLine.substr( 0, poz );           // Zrób substring
            takenLine.erase( 0, poz + 1 );                         // Usuń go wraz z ';'
            RESPONSES.back().push_back( stod(subLine) );           // Wrzucamy do litery jej dane
        }
    }

    sigFile.close();
}

void Teacher::createValidationSet(){

    fstream valFile;
    string takenLine;

    while(true)
    {
        system("cls");
        cout << endl;
        string signalDir = "Signals/" + FILENAME + "v.nsignal";

        valFile.open(signalDir, ios::in);

        if(valFile.good() == true)
        {
            break;
        }
        else{
            cout << endl;
            cout << "\tBrak pliku Walidacyjnego" << endl;
            getchar();
        }
    }

    // Wczytuję SYGNAŁ I ODPOWIEDZI
    while( ! valFile.eof() )
    {
        getline(valFile, takenLine);

        SIGNALS_VAL.push_back(LETTER());                         // Robimy wektor dla litery
        while( ! takenLine.empty() )
        {
            unsigned poz = takenLine.find_first_of(";");         // Znajdź pierwszy znak spec
            string subLine = takenLine.substr(0, poz);           // Zrób substring
            takenLine.erase(0, poz + 1);                         // Usuń go wraz z ';'
            SIGNALS_VAL.back().push_back( stod(subLine) );       // Wrzucamy do litery jej dane
        }

        getline(valFile, takenLine);

        RESPONSES_VAL.push_back(LETTER());
        while( ! takenLine.empty() )
        {
            unsigned poz = takenLine.find_first_of(";");         // Znajdź pierwszy znak spec
            string subLine = takenLine.substr(0, poz);           // Zrób substring
            takenLine.erase(0, poz + 1);                         // Usuń go wraz z ';'
            RESPONSES_VAL.back().push_back( stod(subLine) );     // Wrzucamy do litery jej dane
        }
    }

    valFile.close();
}

void Teacher::teachNetwork()
{
    int epoch = 0;
    EPOCH = epoch;
    while(true)
    {
        bool flag = true;
        while(flag)
        {
            system("cls");
            cout << endl;
            cout << "\t" << "EPOCH\t  =\t" << EPOCH;

            //  TESTOWANIE ZBIORU WALIDACYJNEGO
            for( unsigned E = 0; E < SIGNALS_VAL.size(); E++ )
            {
                networkToTeach->feedForward( SIGNALS_VAL[E] );
                networkToTeach->validator( RESPONSES_VAL[E], RESPONSES_VAL.size() );
            }

            //  UCZENIE SIECI
            for( unsigned E = 0; E < SIGNALS.size(); E++ )
            {
                networkToTeach->feedForward( SIGNALS[E] );
                if( networkToTeach->backProp( RESPONSES[E], RESPONSES.size() ) == false)
                {
                    flag = false;
                }
            }
            EPOCH++;
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
        {
            break;
        }

        for(unsigned x = 0; x < 50; x++)       // Jeśli zostały - staramy się nauczyć sieć ich rozróżniania
        {
            epoch++;
            int randS = rand() % unTeachSignals.size();

            networkToTeach->feedForward(SIGNALS[unTeachSignals[randS]]);
            networkToTeach->backProp(RESPONSES[unTeachSignals[randS]], RESPONSES.size());
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
