#include "dbmanager.h"
#include <sqlite3.h>
#include <iostream>
#include <string>


int cbPrintResults(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " : " << argv[i] << "\t";
    }
    std::cout << std::endl;
    return 0; // success
}

// callback function for counting number of results. or printing everything. i guess
int cbCountResults(void *count, int argc, char **argv, char **azColName) {
    // obtain results from SELECT (user-passed data, number of columns per row, row/field values, column names)
    //std::cout << "Called back:" << std::endl;
    auto counter = static_cast<int*>(count);
    if (counter) {
        (*counter)++;
        // cast to storage structure
        // (https://stackoverflow.com/questions/60811670/how-do-i-store-into-a-c-variables-using-sqlite3)
    }
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " : " << argv[i] << std::endl;
    }
    return 0; // success
}

// callback function for doing nothing
int callback(void* data, int argc, char** argv, char** azColName) {
    return 0;
}

int DbManager::execQuery(std::string query, int (*callback)(void*, int, char**, char**), void* data) {
    char* errMsg = 0;

    int rescode = sqlite3_exec(dbConnection, query.c_str(), callback, data, &errMsg);
    if (rescode) {
        std::cout << "DB error: " << sqlite3_errmsg(dbConnection) << std::endl;
        //sqlite3_close(dbConnection);
        return 1;
    }
    return 0;
}

DbManager::~DbManager() {
    sqlite3_close(dbConnection);
}

DbManager::DbManager(char* dbName) {
    std::cout << "Setting up database connection..." << std::endl;
    int rescode;
    try {
        rescode = sqlite3_open(dbName, &(DbManager::dbConnection));
        if (rescode) {
            throw(rescode);
            sqlite3_close(DbManager::dbConnection);
        }
        std::cout << "Database " << dbName << " connected!" << std::endl;
    } catch (int code) {
        std::cout << "DB error: " << sqlite3_errmsg(DbManager::dbConnection) << " with rescode " << code << std::endl;
        return;
    }

    // if first startup, run setup and stuff (todo remove repeated code)
    int countResults = 0;
    char* errMsg = 0; // space for SQLite to place an error message if one occurs
    std::string dbCheck = "SELECT name FROM sqlite_master";
    rescode = sqlite3_exec(dbConnection, dbCheck.c_str(), cbCountResults, &countResults, &errMsg);

    if (rescode) {
        std::cout << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else {
        if (!countResults) {
            newTables();
        }
    }
}

int DbManager::newTables() {
    std::cout << "Setting up new tables..." << std::endl;
    
    // sqlite integer primary key implicit autoincrement
    std::string query = std::string("CREATE TABLE Tasks ") +
     "(TASK_ID INTEGER PRIMARY KEY NOT NULL," +
     "TASK_NAME TEXT NOT NULL," + 
     // "COMPLETED INT DEFAULT 0," +
     "PRIORITY TEXT CHECK( PRIORITY IN ('HIGH', 'MED', 'LOW')))";
    //std::cout << query << std::endl;
    
    execQuery(query, cbCountResults, 0);
    return 0;
}

void DbManager::loadTasks() {
    std::cout << "Getting tasks..." << std::endl;
    std::string query = std::string("SELECT * FROM Tasks");

    execQuery(query, cbPrintResults, 0);
}

// finds a task by their id (todo, overload with find tasks by name or whatever too)
//Task DbManager::getTask(int i) {
//    std::cout << "Finding task..." << std::endl;
//    std::string query = std::string("");
//
//    execQuery(query, cbPrintResults, 0);
//}

// puts a new task in, todo edit a task if its id is already there
int DbManager::putTask(Task t) {
    std::string query = std::string("INSERT INTO Tasks (TASK_ID, TASK_NAME, PRIORITY) ") +
        "VALUES(" + (t.id < 0 ? "NULL" : std::to_string(t.id)) + ",\"" + t.name + "\",\"" + t.priority + "\")";

    execQuery(query, callback, 0);
    std::cout << "Task inserted! (theoretically)" << std::endl;
    return 0;
}

int DbManager::removeTask(Task t) {
    std::cout << "Removing task..." << std::endl;

    std::string query = std::string("DELETE FROM Tasks ") +
        "WHERE TASK_ID = " + std::to_string(t.id);

    execQuery(query, callback, 0);
    std::cout << "Task removed! (theoretically)" << std::endl;
    return 0;
}

// Task Management
