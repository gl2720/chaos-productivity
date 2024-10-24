#pragma once
#include <string>
#include <sqlite3.h>
#include "task.h"

class DbManager {
    public:
    sqlite3* dbConnection;
    //int numResults = 0;
    //char* lastQuery;

    DbManager(char* dbName);
    ~DbManager();

    // tasks, focus sessions, routines (time-based yes/no achievements. as opposed to completion-based achievements (tasks))
    // keep completed things in their own db and include an Upcoming db with all their ids?

    // multiple callbacks to handle... stuff (callbacks are generally static functions)

    //getItem(int i); // re-executes query but returns said item (in some form)
    //putItem(); // puts it into the database. i guess
    //updateItem(); // yeah...
    //removeItem();

    // tasks
    void loadTasks(); // may get more specific in the future
    Task getTask(int i);
    int putTask(Task t); // new or update
    int removeTask(Task t);

    // sessions
    // start, stop, pause

    // routines
    // load routines, complete

    // tasks
    // int newTask();
    // int editTask();
    // int removeTask();
    // Task* loadCompleted();
    // Task* loadAllTasks();

    // character properties (alt file storage types: ini file, but why would i make it so complicated for no reason)
    // char* getValue(char* key);
    // char* putValue(char* key);

    private:
    int newTables(); // this isn't really necessary in the end, probably just pre-create everything you need
    // int newUser(char* userName);
};