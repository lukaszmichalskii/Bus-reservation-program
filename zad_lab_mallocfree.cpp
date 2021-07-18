#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <cstdio>

using namespace std;

// function declaration
int ** alloc2D(int, int);
void * alloc(int);
void dealloc(void*);
void textColor(int);
bool isRightCIN();
bool isRightSEATSnr(int, int);
bool isRightBUSnr(int, int);
void busesState(int **, int, int);
void bus(int **, int, int);
bool isRightSEATnr(int, int);
bool seatReservation(int **, int, int);
bool isRightBUS_FULL(int **, int, int);
bool isRightDecision(char);
void numberOfFreeSEATS(int **, int, int, int*);

// Reservation program for places in buses
int main()
{
    // one of requirement of laboratory project - dynamic memory allocation
    int ** busDatabase;
    int * numOfSeats;
    int * numOfBuses;
    int * seats;
    int * allSeats;
    int * busSelection;
    int * seatSelection;
    int * freeSeats;
    char * decision;

    numOfBuses = (int*)alloc(sizeof(int));
    numOfSeats = (int*)alloc(sizeof(int));

    cout<<"Number of buses in company: ";
    do {cin>>*numOfBuses;} while(!isRightCIN());
    cout<<"Number of seats in one bus: ";
    do {cin>>*numOfSeats;} while(!isRightCIN());

    // allocating memory for a bus database

    if ((busDatabase = alloc2D(*numOfBuses, *numOfSeats)) == nullptr)
    {
        textColor(4);
        cout<<"ERROR Allocating Memory!";
        textColor(7);
        exit(1);
    }

    system("cls");

    cout<<"Bus seats reservation program\n";
    cout<<"\nBuses database:\n";

    //Buses database
    busesState(busDatabase, *numOfBuses, *numOfSeats);

    // variables allocation process
    try
    {
        busSelection = (int*)alloc(sizeof(int));
        seatSelection = (int*)alloc(sizeof(int));
        seats = (int*)alloc(sizeof(int));
        allSeats = (int*)alloc(sizeof(int));
        decision = (char*)alloc(sizeof(char));
        freeSeats = (int*)alloc(sizeof(int));
    }
    catch(...)
    {
        textColor(4);
        cout<<"ERROR Allocating Memory!";
        textColor(7);
        exit(1);
    }

    *decision = 'N';
    numberOfFreeSEATS(busDatabase, *numOfBuses, *numOfSeats, freeSeats);

    // main program loop
    while(*decision == 'N' || *decision == 'n')
    {
        cout << "Enter how many seats you want to reserve (number of available seats = " << *freeSeats << "): ";

        do
        {
            cin>>*seats;
            if (*seats == *freeSeats)
            {
                for (int i = 0; i<*numOfBuses; i++)
                    for (int j = 0; j<*numOfSeats; j++)
                        busDatabase[i][j]=1;
                system("cls");
                cout<<"Automatic seat reservation was made"<<endl<<endl;

                // requirement of creating laboratory project use goto instruction.
                // Understand why it is undesirable
                goto end;
            }
        } while(!isRightCIN() || !isRightSEATSnr(*seats, *freeSeats));

        *allSeats = *seats;


        while(*seats)
        {

            if (*seats < *allSeats)
                cout << "\nThe number of seats you wanted to reserve " << *allSeats << ". Places left to book = " << *seats << endl;
            cout<<"Select the bus you want to reserve a seat for: ";

            do
            {
                cin>>*busSelection;
            }while(!isRightCIN() || !isRightBUSnr(*busSelection, *numOfBuses) || !isRightBUS_FULL(busDatabase, *busSelection, *numOfSeats)) ;

            bus(busDatabase, *numOfSeats, *busSelection);
            cout<<"\nEnter the number of the seat you want to reserve: ";

            do{cin>>*seatSelection;}while(!isRightCIN() || !isRightSEATnr(*seatSelection, *numOfSeats) || !seatReservation(
                    busDatabase, *busSelection, *seatSelection));

            system("cls");
            textColor(10);
            cout<<"Seat booked correctly\n";
            textColor(7);
            cout<<"Bus condition after booking\n";

            // buses state
            busesState(busDatabase, *numOfBuses, *numOfSeats);

            *seats -= 1;
            *freeSeats -= 1;
        }
        if (*freeSeats > 0)
            cout<<"Do you want to end? <T/N> ";
        else
        {
            system("cls");
            cout<<"All available seats have been taken"<<endl;
            goto end;
        }
        do{cin>>*decision;}while(!isRightCIN() || !isRightDecision(*decision));
    }




    system("cls");
    end:
    textColor(10);
    cout<<"Thank you for booking"<<endl;
    textColor(7);
    cout<<"Bus condition after booking\n";

    // Bus state
    busesState(busDatabase, *numOfBuses, *numOfSeats);


    cout<<"Press ENTER to end the program";
    getchar(); getchar();

    // deallocating memory
    dealloc(numOfSeats);
    dealloc(numOfBuses);
    dealloc(seats);
    dealloc(busSelection);
    dealloc(seatSelection);
    dealloc(freeSeats);
    dealloc(decision);


    // clear with one 'free' (the alloc2D function calls 'malloc' once and reserved memory for the entire database, so calling 'free' will clean the whole database)
    free(busDatabase);

    return 0;
}



/*
PROGRAM FUNCTIONS
*/


int ** alloc2D(int rows, int cols)
{
    // memory allocating for database
    int ** database;
    int rowsSize = rows * sizeof(int*);
    int data = rows * cols * sizeof(int);

    if ((database = (int**)malloc(rowsSize + data)) == nullptr)
        return nullptr;

    // seat settings for buses
    int * wsk = (int*)(database + rows);
    for (int i = 0; i < rows; i++)
        database[i] = wsk + i * cols;

    /*
    'malloc' does NOT reset allotted memory! ---------- 'calloc' YES!
    in this case you have to use memset to "clear the content",
    otherwise there may be a segmentation error (access to the reserved memory area)
    */

    memset(&database[0][0], 0, data);

    return database;
}


void * alloc(int size)
{
    // memory allocation for variables

    void * variable;

    if ((variable = (void*)malloc(sizeof(size))) == nullptr)
    {
        textColor(4);
        throw "\nERROR Allocating Memory";
    }

    return variable;
}

void dealloc(void * variable)
{
    // memory deallocating for variables
    if (variable == nullptr)
        return
                free(variable);
}


void textColor(int colorCode)
{
    // change text color in console

    HANDLE form;
    form = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(form, colorCode);
}

bool isRightCIN()
{
    // checking if the data has been correctly loaded into the CIN >> stream

    if (cin.rdstate())
    {
        cin.clear();
        cin.ignore(1024, '\n');
        textColor(4);
        cout<<"Invalid data, please try again: ";
        textColor(7);
        return false;
    }

    return true;
}

bool isRightSEATSnr(int seats, int freeSeats)
{
    // checking if seat data has been loaded correctly

    if (seats > freeSeats)
    {
        textColor(4);
        cout<<"Error! We don't have that many seats, please try again: ";
        textColor(7);
        return false;
    }

    else if (seats < 0)
    {
        textColor(4);
        cout<<"Error! The number of seats cannot be negative, try again: ";
        textColor(7);
        return false;
    }

    //	else if (seats == 0)
    //	{
    //		textColor(10);
    //		cout<<"Thank you for using the program";
    //		textColor(7);
    //
    //	}

    return true;
}

bool isRightBUSnr(int busSelection, int numOfBus)
{
    // checking if the Bus number has been correctly read

    if (0 >= busSelection || busSelection > numOfBus)
    {
        textColor(4);
        cout<<"Error! Bus number entered incorrectly, try again: ";
        textColor(7);
        return false;
    }
    return true;
}

void busesState(int ** database, int numOfBuses, int numOfSeats)
{
    // displaying the updated database, seats occupied, free, etc.

    cout<<"-----------------------------------------\n*Marked places \"0\" are free\n*Marked places \"1\" are reserved\n-----------------------------------------\n";

    for (int w = 0; w < numOfBuses; w++)
    {
        cout<<"Bus nr. ";
        cout.width(2);
        cout<<right<<w+1;
        cout<<" --> ";
        for (int k = 0; k < numOfSeats; k++)
        {
            if (database[w][k] == 0)
                cout<<database[w][k]<<" ";
            else
            {
                textColor(4);
                cout<<database[w][k]<<" ";
                textColor(7);
            }

        }

        cout<<endl;
    }
    cout<<endl;
}

void bus(int ** database, int numOfSeats, int busNumber)
{
    // printing single bus

    cout.width(35);
    cout<<left<<"seat nr";

    for (int i = 0; i < numOfSeats; i++)
    {
        cout.width(2);
        cout<<right<<i+1<<" ";
    }

    cout<<endl;
    cout.width(35);
    cout<<left<<"seat state (0-free, 1-reserved)";
    for (int i = 0; i < numOfSeats; i++)
    {
        cout.width(2);
        if (database[busNumber - 1][i] == 1)
        {
            textColor(4);
            cout << right << database[busNumber - 1][i] << " ";
            textColor(7);
        }
        else
            cout << right << database[busNumber - 1][i] << " ";
    }
}

bool isRightSEATnr(int seatSelection, int numOfSeats)
{
    // checking if the seat number for reservation has been correctly loaded

    if (seatSelection <= 0 || seatSelection > numOfSeats)
    {
        textColor(4);
        cout<<"Error! Seat number incorrectly entered, try again: ";
        textColor(7);
        return false;
    }

    return true;
}

bool isRightBUS_FULL(int ** database, int busNumber, int numOfSeats)
{
    // checking if the bus is already full

    for (int i = 0; i < numOfSeats; i++)
    {
        if (database[busNumber - 1][i] == 0)
        {
            return true;
        }
    }
    textColor(4);
    cout<<"Bus full, try again: ";
    textColor(7);
    return false;
}

bool isRightDecision(char decision)
{
    // function checking loaded data - ending the program by the user

    if (decision != 'T' && decision != 't' && decision != 'N' && decision != 'n')
    {
        textColor(4);
        cout<<"Error! Invalid value\n";
        textColor(7);
        cout<<"YES - enter 'T' lub 't'\nNO - enter 'N' lub 'n'\nTry again: ";
        return false;
    }
    return true;
}


bool seatReservation(int ** database, int busSelection, int seatSelection)
{
    // checking the availability of the reserved seat

    if (database[busSelection - 1][seatSelection - 1] == 1)
    {
        textColor(4);
        cout<<"The place is busy! Try again: ";
        textColor(7);
        return false;
    }
    else if (database[busSelection - 1][seatSelection - 1] == 0)
    {
        database[busSelection - 1][seatSelection - 1] = 1;
        return true;
    }
    else
    {
        cout<<"Error! An invalid value has been entered";
        return false;
    }
}

void numberOfFreeSEATS(int ** database, int numOfBuses, int numOfSeats, int * buf)
{
    // counting the amount of available seats on the bus.

    *buf = 0;
    for (int i = 0; i < numOfBuses; i++)
        for (int j = 0; j < numOfSeats; j++)
            if(database[i][j] == 0)
                ++*buf;
}
