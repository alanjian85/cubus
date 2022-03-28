#include "database.h"
using namespace cephalon;

#include "world.h"

#include <iostream>

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
    const char* insert_query = "REPLACE INTO blocks (x, y, z, name) VALUES (?, ?, ?, ?)";
    const char* load_query = "SELECT x, y, z, name FROM blocks;";
    sqlite3_exec(db_, create_query, nullptr, nullptr, nullptr);
    sqlite3_prepare(db_, insert_query, -1, &insert_stmt_, nullptr);
    sqlite3_prepare(db_, load_query, -1, &load_stmt_, nullptr);
}

Database::~Database() {
    sqlite3_finalize(insert_stmt_);
    sqlite3_close(db_);
}

std::unordered_map<glm::ivec3, const Block*> Database::loadBlocks() const {
    std::unordered_map<glm::ivec3, const Block*> blocks;
    while (sqlite3_step(load_stmt_) == SQLITE_ROW) {
        auto x = sqlite3_column_int(load_stmt_, 0);
        auto y = sqlite3_column_int(load_stmt_, 1);
        auto z = sqlite3_column_int(load_stmt_, 2);
        auto name = sqlite3_column_text(load_stmt_, 3);
        blocks[glm::ivec3(x, y, z)] = Block::getBlock(reinterpret_cast<const char*>(name));
    }
    return blocks;
}

void Database::insertBlock(glm::ivec3 pos, const char* name) {
    sqlite3_reset(insert_stmt_);
    sqlite3_bind_int(insert_stmt_, 1, pos.x);
    sqlite3_bind_int(insert_stmt_, 2, pos.y);
    sqlite3_bind_int(insert_stmt_, 3, pos.z);
    sqlite3_bind_text(insert_stmt_, 4, name, -1, nullptr);
    sqlite3_step(insert_stmt_);
}