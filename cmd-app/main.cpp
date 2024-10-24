#include <iostream>
#include <string> // c++
#include <random>
#include "dbmanager.h"

// let each function accept signals for stop and pause-- after all only one function can be ongoing (plus main ui)

using namespace std;

const char* salutations[] = {
    "hi dude.",
    "hey.",
    "what's up?",
    "hi.",
    "i don't really have a visual form but just imagine i'm waving nicely at you. hope you're well."
};

const char* farewells[] = {
    "bye.",
    "see you soon.",
    "i'll see you.",
    "later.",
    "seeya."
};

void greeting() {
    // uniform initialization in c++
    std::uniform_int_distribution<> dist(1, sizeof(salutations)/sizeof(salutations[0]) - 1);
    std::mt19937 gen(std::time(NULL));
    cout << salutations[dist(gen)] << endl;
    // if you have a wakeup routine then put in something timebased maybe
}

int acceptAnswer(char* a, char* b) {
    string userin;
    cout << "0: " << a << endl;
    cout << "1: " << b << endl;
    getline(cin, userin);
    if (userin != std::string("0") && userin != std::string("1")) {
        cout << "i'm stuck in your computer right now-- i can't process anything except 0 or 1. please." << endl;
        getline(cin, userin);
        if (userin != std::string("0") && userin != std::string("1")) { // 2 tries
            cout << "whatever." << endl;
            return -1;
        }
    }
    return std::stoi(userin);
}

int main() {
    cout << "Waking up..." << endl;
    DbManager db = DbManager::DbManager("chaos.db");
    string userin;
    // greeting sequence: run daily checks or something
    greeting();
    cout << "here's what you need to do today." << endl;
    db.loadTasks();
    
    cout << "is there anything else?" << endl;
    int ans = acceptAnswer("yes. please let me add a new task.", "no, that's all, thank you.");
    if (ans == 0) {
        cout << "alright, hit me." << endl;
        string newtask;
        getline(cin, newtask);
        Task t(newtask.c_str());
        db.putTask(t);
    }
    else {
        cout << "great. ummm. i'll have more things for you to do later. bye now." << endl;
    }

    return 0;
}