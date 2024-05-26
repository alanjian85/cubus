#pragma once

#include <mutex>
#include <unordered_map>
#include <string>

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
        void deleteBlock(glm::ivec3 pos);
    private:
        static int loadIdCallback(void* self, int, char** texts, char**);
        
        int getBlockId(const char* name);

        sqlite3* db_;

        mutable std::mutex blocks_id_mutex_;
        std::unordered_map<std::string, int> blocks_id_;
        std::unordered_map<int, std::string> blocks_name_;

        mutable std::mutex insert_id_mutex_;
        sqlite3_stmt* insert_id_stmt_;
        
        mutable std::mutex insert_mutex_;
        sqlite3_stmt* insert_stmt_;

        mutable std::mutex load_mutex_;
        sqlite3_stmt* load_stmt_;

        mutable std::mutex delete_mutex_;
        sqlite3_stmt* delete_stmt_;
    };
}
