#include "database.h"
using namespace cephalon;

#include "world.h"

#include <iostream>

#include <spdlog/spdlog.h>

Database::Database(const char* path) {
    int rc;
    rc = sqlite3_open(path, &db_);
    if (rc) {
        spdlog::error("Couldn't open database {}", path);
        return;
    } else {
        spdlog::info("Database {} is opened", path);
    }
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
    rc = sqlite3_exec(db_, create_query, nullptr, nullptr, nullptr);
    if (rc) spdlog::error("Failed to execute table create query");
    else spdlog::info("Table created successfully");
    rc = sqlite3_prepare(db_, insert_query, -1, &insert_stmt_, nullptr);
    if (rc) spdlog::error("Failed to prepare block insert statement");
    else spdlog::info("Block insert statement compiled successfully");
    rc = sqlite3_prepare(db_, load_query, -1, &load_stmt_, nullptr);
    if (rc) spdlog::error("Failed to prepare chunk load statement");
    else spdlog::info("Chunk load statement compiled successfully");
}

Database::~Database() {
    int rc;
    rc = sqlite3_finalize(load_stmt_);
    if (rc) spdlog::critical("Failed to finalize chunk load statement");
    else spdlog::info("Chunk load statement finalized");
    rc = sqlite3_finalize(insert_stmt_);
    if (rc) spdlog::critical("Failed to finalize block insert statement");
    else spdlog::info("Block insert statement finalized");
    rc = sqlite3_close(db_);
    if (rc) spdlog::critical("Failed to close database");
    else spdlog::info("Database closed");
}

void Database::loadChunk(Chunk& chunk) const {
    std::lock_guard lock(load_mutex_);
    sqlite3_reset(load_stmt_);
    while (sqlite3_step(load_stmt_) == SQLITE_ROW) {
        auto x = sqlite3_column_int(load_stmt_, 0);
        auto y = sqlite3_column_int(load_stmt_, 1);
        auto z = sqlite3_column_int(load_stmt_, 2);
        glm::ivec3 pos(x, y, z);
        auto name = sqlite3_column_text(load_stmt_, 3);
        auto block = Block::getBlock(reinterpret_cast<const char*>(name));
        if (!block) {
            spdlog::warn("Unknown block name {}", name);
            continue;
        }
        if (World::getRegion(pos) == chunk.getRegion()) {
            chunk.setBlock(World::getOffset(pos), *block);
        }
    }
}

void Database::insertBlock(glm::ivec3 pos, const char* name) {
    std::lock_guard lock(insert_mutex_);
    sqlite3_reset(insert_stmt_);
    sqlite3_bind_int(insert_stmt_, 1, pos.x);
    sqlite3_bind_int(insert_stmt_, 2, pos.y);
    sqlite3_bind_int(insert_stmt_, 3, pos.z);
    sqlite3_bind_text(insert_stmt_, 4, name, -1, nullptr);
    int rc = sqlite3_step(insert_stmt_);
    if (rc != SQLITE_DONE) spdlog::critical("Failed to execute block insert statement");
    else spdlog::debug("Block inserted successfully");
}