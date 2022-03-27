#include "database.h"
using namespace cephalon;

Database::Database(const char* path) {
    sqlite3_open(path, &db_);
    const char* create_query =
        "CREATE TABLE IF NOT EXISTS blocks ("
        "x    INT"
        "y    INT"
        "z    INT"
        "name TEXT"
        ");";
    sqlite3_exec(db_, create_query, nullptr, nullptr, nullptr);
}

Database::~Database() {
    sqlite3_close(db_);
}