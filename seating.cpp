#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>

using namespace std;

class table;

class student
{
    public:
        //add partners who must sit together
        string name;
        vector<student*> friends;
        vector<student*> not_friends;
        table* student_table;
        bool seated;
        void addFriends(student *studen);
        void addEnemies(student *studen);
        void printFriends();
        void printEnemies();
};

class table
{
    public:
        table(int nS) : numSeats(nS) { }
        int tableNum;
        int numSeats;
        vector<student*> seats;
        vector<student*> restricted;
        void printRestricted();
        void printTable();
        bool addStudent(student *s);
};

class classroom
{
    public:
        student* find(string studen);
        void buildClassroom();
        void redistribute();
        void getRoomDetails();
        void createTables();
        void printRoster();
        void printTables();
        int num_tables;
        int seats_per_table;
        vector<table*> tables;
        int numTables() { return tables.size(); }
        vector<student*> roster;    
        bool addStudentToTable(student *s);
};

void table::printRestricted()
{

    vector<student*>::iterator itr = restricted.begin();
    cout << "Restricted at table: " << tableNum << endl;
    for(itr; itr != restricted.end(); itr++)
    {
        cout << *itr << endl;
    }

}

void classroom::redistribute()
{
    student* tmp;
    vector<table*>::iterator itr = tables.begin();
    vector<student*>::iterator itr2 = (*itr)->restricted.begin();
    for(itr; itr != tables.end(); itr++)
    {
        for(itr2; itr2 != (*itr)->restricted.end(); itr2++) {
            if(std::find((*itr)->seats.begin(), (*itr)->seats.end(), *itr2) != (*itr)->seats.end()) {
                cout << "Student "<< (*itr2)->name << " cannot sit at this table." << endl;
            }
        }
    }
}

student* classroom::find(string studen)
{
    vector<student*>::iterator itr = roster.begin();
    for(itr; itr != roster.end(); itr++) {
        if( ((*itr)->name).compare(studen) == 0) {
            return *itr;
        }
    }
    return NULL;
}

void classroom::buildClassroom()
{
    int tbl = 1;
    cout << "Bulding default room" << endl;
    tables.clear();
    getRoomDetails();
    createTables();
    vector<student*>::iterator itr = roster.begin();
    vector<table*>::iterator itr2 = tables.begin();
    for(itr2; itr2 != tables.end(); itr2++) {
        while(itr != roster.end()){
            if((*itr2)->numSeats > 0) {
                (*itr)->student_table = (*itr2);
                (*itr2)->seats.push_back(*itr);
                (*itr2)->numSeats--;
                itr++;
            }
            else
                break;
        }
        tbl++;
    }
    printTables();
}

void classroom::getRoomDetails()
{
    cout << "How many tables are in the room? ";
    cin >> num_tables;
    cout << "How many students can fit at each table? ";
    cin >> seats_per_table;
}

void classroom::printRoster()
{
    cout << "Roster:" << endl;
    for(vector<student*>::iterator itr = roster.begin(); itr != roster.end(); itr++)
    {
        cout << (*itr)->name << endl;
    }
}

void table::printTable()
{
    vector<student*>::iterator itr = seats.begin();
    cout << "Students currently seated at table:" << endl;
    for(itr; itr != seats.end(); itr++)
        cout << (*itr)->name << endl;
    itr = restricted.begin();
    cout << endl;
    cout << "Students currently restricted from sitting at table:" << endl;
    for(itr; itr != restricted.end(); itr++)
    {
        cout << (*itr)->name << endl;
    }
}

void classroom::printTables()
{
    vector<table*>::iterator itr = tables.begin();
    for(itr; itr != tables.end(); itr++)
    {
        cout << "Table: " << (*itr)->tableNum << endl;
        (*itr)->printTable();
        cout << endl;
    }
}

void student::addFriends(student *new_friend)
{
    if(new_friend)
        friends.push_back(new_friend); 
    else
        cout << "Student not found." << endl;
}

void student::printFriends()
{
    cout << "Student: " << name << endl;
    cout << "Friends: " << endl;
    vector<student*>::iterator itr = friends.begin();
    for(itr; itr != friends.end(); itr++)
    {
        cout << (*itr)->name << endl;
    }
    cout << endl;
}

void student::printEnemies()
{
    cout << "Student: " << name << endl;
    cout << "Enemies: " << endl;
    vector<student*>::iterator itr = not_friends.begin();
    for(itr; itr != not_friends.end(); itr++)
    {
        cout << (*itr)->name << endl;
    }
    cout << endl;
}

void student::addEnemies(student *enemy)
{
    if(enemy) {
        not_friends.push_back(enemy);
        if(student_table) {
            (student_table->restricted).push_back(enemy);
        }
    }
    else
        cout << "Student not found." << endl;
}

bool classroom::addStudentToTable(student* s)
{
    //restricted.insert(restricted.end(), s->not_friends.begin(), s->not_friends.end());
    for(vector<student*>::iterator itr = roster.begin(); itr != roster.end(); itr++)
    {
        if((*itr)->seated)
            continue;
        for(vector<table*>::iterator itr2 = tables.begin(); itr2 != tables.end(); itr2++)
        {
            for(vector<student*>::iterator itr3 = (*itr2)->restricted.begin(); itr3 != (*itr2)->restricted.end(); itr3++)
            {
                if((*itr) == (*itr3))
                {
                    cout << "Student " << s->name << "cannot sit at this table." << endl;
                    break;
                }
                else
                {
                    (*itr2)->seats.push_back(s);
                    s->seated = true;
                    (*itr2)->numSeats--;
                }
            }
        }
    }
}

void classroom::createTables()
{
    int tbl = 1;
    while(num_tables > 0)
    {
        table *t = new table(seats_per_table);
        t->tableNum = tbl;
        tables.push_back(t);
        num_tables--;
        tbl++;
    }
}


void printMenu()
{

    cout << "0. Exit." << endl;
    cout << "1. Print roster." << endl;
    cout << "2. Build classroom." << endl;
    cout << "3. Add friends to student." << endl;
    cout << "4. Add enemies to student." << endl;
    cout << "5. Print tables." << endl;
    cout << "6. Redistribute students. " << endl;
    cout << "7. Export Classroom." << endl;

}

int getChoice()
{
    int choice;
    printMenu();
    cout << "Select option: ";
    cin >> choice;
    cout << endl;
    return choice;
}

void menuSelect(classroom &my_room, int choice)
{
    switch(choice)
    {
        case 0:
        {
            cout << "Thank you for using this software. Goodbye." << endl;
            exit(0);
            break;
        }
        case 1:
        {
            my_room.printRoster();
            break;
        }
        case 2:
        {
            my_room.buildClassroom();
            break;
        }
        case 3:
        {
            //add friends to student
            //implement hash table for student lookups
            string studen, new_frien;
            student *student, *new_friend;
            //end line somewhere FIX
            cin.get();

            cout << "Name of student with new friends: ";
            getline(cin,studen);
            //hash table needed
            student = my_room.find(studen);
            if(!student) {
                cout << "Student not found." << endl;
                break;
            }
            cout << "Name of new friend: ";
            getline(cin,new_frien);
            new_friend = my_room.find(new_frien);
            if(!new_friend) {
                cout << "Student not found." << endl;
                break;
            }
            student->addFriends(new_friend);
            student->printFriends();
            printMenu();
            break;
        }
        case 4:
        {
            //add enemies to student
            //implement hash table for student lookups
            string studen, new_enemy;

            //end line somewhere FIX
            cin.get();

            cout << "Name of student with new enemy: ";
            getline(cin,studen);
            //hash table needed
            student* student = my_room.find(studen);
            cout << "Name of new enemy: ";
            getline(cin,new_enemy);
            student->addEnemies(my_room.find(new_enemy));
            student->printEnemies();
            break;
        }
        case 5:
        {
            //print tables
            my_room.printTables();
            break;
        }
        case 6:
        {
            //add case
            my_room.redistribute();
            break;
        }
        default:
        {
            cout << "Select menu option" << endl;
            break;
        }
    }
    cout << endl << endl;
}

int main()
{
    classroom my_room;
    ifstream roster;
    roster.open("roster.txt");
    string first, last;
    while(roster >> first >> last) {
        student *s = new student;
        s->name = first + " " + last;
        my_room.roster.push_back(s);
    }

    my_room.buildClassroom();

    while(true)
        menuSelect(my_room, getChoice());

    //my_class->roster.begin()->restricted.push_back("1");

    //my_class.printRoster();

    /*for(vector<table*>::iterator itr = my_class->tables.begin(); itr != my_class->tables.end(); itr++)
    {
        while((*itr)->numSeats > 0)
        {
            //(*itr)->addStudentToTable( *(my_class->roster.begin()) );
        }
    }*/

    my_room.printTables();
    
}
