#include "dbmanager.h"
#include <sqlite3.h>
#include <iostream>
#include <string>


// callback function for counting number of results
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

    // if first startup, run setup and stuff
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

    // DbManager::newTables();
    // sqlite3_close(dbConnection);
}

int DbManager::newTables() {
    std::cout << "Setting up new tables..." << std::endl;
    char *errMsg = 0;
    // sqlite integer primary key autoincrements implicitly even without the 'autoincrement' keyword
    // if the value is not specified on insert (if specifically it is null)
    // (the column must be Integer not Int)
    std::string query = std::string("CREATE TABLE Tasks ") +
     "(TASK_ID INTEGER PRIMARY KEY NOT NULL," +
     "TASK_NAME TEXT NOT NULL," + 
     "PRIORITY TEXT CHECK( PRIORITY IN ('HIGH', 'MED', 'LOW')))";
    //std::cout << query << std::endl;
    
    int rescode = sqlite3_exec(dbConnection, query.c_str(), callback, 0, &errMsg);
    if (rescode) {
        std::cout << "DB error: " << sqlite3_errmsg(DbManager::dbConnection) << std::endl;
        sqlite3_close(dbConnection);
        return 1;
    }
    return 0;
}

// Task Management
