// Author: Ivan Arizanovic <ivanarizanovic@yahoo.com>

#pragma once
#include <sqlite3.h>
#include <iostream>
#include <queue>
#include <string>

class Database {
public:
    //--------------------------------------------------------------------------
    // Save received message to database
    // @param message   Input message
    // @return          Error code from SQLite
    //--------------------------------------------------------------------------
    static int persistMessageInSQLite(const std::string& message);

    //--------------------------------------------------------------------------
    // Get the latest message
    // @return          The latest message
    //--------------------------------------------------------------------------
    static std::string getLatestValue();

private:
    //--------------------------------------------------------------------------
    // Execute the states
    // @param states    States to execute
    // @return          Error code from SQLite
    //--------------------------------------------------------------------------
    static int SQLiteProcess(std::queue<std::string>& states);

    //--------------------------------------------------------------------------
    // Callback function from SQLite
    //--------------------------------------------------------------------------
    static int callback(void* NotUsed, int argc, char** argv, char** azColName);
};
