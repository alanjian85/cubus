#ifndef CEPHALON_DATABASE_H_
#define CEPHALON_DATABASE_H_

#include <mutex>
#include <unordered_map>

#include <glm/glm.hpp>
#include <sqlite3.h>

#include "blocks/block.h"
#include "chunk.h"

namespace cephalon {
    class World;

    class Database {
    public:
        Database(const char* path);

        Database(const Database&) = delete;

        Database& operator=(const Database&) = delete;

        ~Database();

        void loadChunk(Chunk& chunk) const;

        void insertBlock(glm::ivec3 pos, const char* name);    
    private:
        sqlite3* db_;

        mutable std::mutex load_mutex_;
        sqlite3_stmt* load_stmt_;
        
        mutable std::mutex insert_mutex_;
        sqlite3_stmt* insert_stmt_;
    };
}

#endif // CEPHALON_DATABASE_H_