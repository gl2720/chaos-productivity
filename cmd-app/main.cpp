#include <iostream>
#include <string> // c++ header, the alternative "string.h" is technically a C header
// #include <ncurses.h>
#include "dbmanager.h"

// let each function accept signals for stop and pause-- after all only one function can be ongoing (plus main ui)

using namespace std;

void greeting() {
    cout << "hi ";
    cout << "dude." << endl;
}

int acceptAnswer(char* a, char* b) {
    string userin;
    cout << "0: " << a << endl;
    cout << "1: " << b << endl;
    getline(cin, userin);
    return 0;
}

int main() {
    string userin;
    // greeting sequence: run daily checks or something
    greeting();
    // cout << "here's what you need to do today." << endl;
    // load tasks for today
    DbManager db = DbManager::DbManager("chaos.db");
    /*cout << "is there anything else you need?" << endl;
    getline(cin, userin);
    cout << "you wanted " << userin << "?" << endl;*/
    return 0;
}