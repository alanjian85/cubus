#include "database.h"
using namespace cephalon;

Database::Database(const char* path) {
    sqlite3_open(path, &db_);
    const char* create_query =
        "CREATE TABLE IF NOT EXISTS blocks ("
        "x    INT NOT NULL,"
        "y    INT NOT NULL,"
        "z    INT NOT NULL,"
        "name TEXT NOT NULL"
        ");"
        "CREATE UNIQUE INDEX IF NOT EXISTS blocks_xyz_idx ON blocks (x, y, z)";
    const char* insert_query = 
        "REPLACE INTO blocks (x, y, z, name) VALUES (?, ?, ?, ?)";
    sqlite3_exec(db_, create_query, nullptr, nullptr, nullptr);
    sqlite3_prepare(db_, insert_query, -1, &insert_stmt_, nullptr);
}

Database::~Database() {
    sqlite3_finalize(insert_stmt_);
    sqlite3_close(db_);
}

void Database::insert_block(glm::ivec3 pos, const char* name) {
    sqlite3_reset(insert_stmt_);
    sqlite3_bind_int(insert_stmt_, 1, pos.x);
    sqlite3_bind_int(insert_stmt_, 2, pos.y);
    sqlite3_bind_int(insert_stmt_, 3, pos.z);
    sqlite3_bind_text(insert_stmt_, 4, name, -1, nullptr);
    sqlite3_step(insert_stmt_);
}