// Author: Ivan Arizanovic <ivanarizanovic@yahoo.com>

#include "Database.h"

int
Database::callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    std::cout << "\n";

    return 0;
}

int 
Database::SQLiteProcess(std::queue<std::string>& states) {
    sqlite3* db;
    char* zErrMsg = 0;
    int retc;

    // Open the database
    retc = sqlite3_open("Messages.db", &db);
    if (retc) {
        std::cout << "Database cannot be opened: " << sqlite3_errmsg(db) << std::endl;
        return retc;
    }

    // Execute the states
    while (!states.empty()) {
        retc = sqlite3_exec(db, states.front().c_str(), NULL, 0, &zErrMsg);
        states.pop();
        if (retc != SQLITE_OK) {
            std::cout << "SQL error: " << sqlite3_errmsg(db) << "\n";
            sqlite3_free(zErrMsg);
            break;
        }
    }

    sqlite3_close(db);
    return retc;
}

int 
Database::persistMessageInSQLite(const std::string& message) {
    std::queue<std::string> states;

    // Create the table if not exists
    states.push("CREATE TABLE IF NOT EXISTS messages (id INTEGER PRIMARY KEY, message TEXT)");

    // Insert the message
    states.push("INSERT INTO messages (message) VALUES ('" + message + "')");

    return SQLiteProcess(states);
}

std::string
Database::getLatestValue() {
    sqlite3* db;
    sqlite3_stmt* stmt;
    int retc;

    // Open the database
    retc = sqlite3_open("Messages.db", &db);
    if (retc) {
        std::cout << "Database cannot be opened: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    // Select the latest value
    sqlite3_prepare(db, "SELECT * FROM messages ORDER BY id DESC LIMIT 1;", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, 16);

    std::string output("");
    if (sqlite3_step(stmt) == SQLITE_ROW)
        output = (char*)sqlite3_column_text(stmt, 1);

    sqlite3_finalize(stmt);

    return output;
}
