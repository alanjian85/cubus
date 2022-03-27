#ifndef CEPHALON_DATABASE_H_
#define CEPHALON_DATABASE_H_

#include <glm/glm.hpp>
#include <sqlite3.h>

namespace cephalon {
    class Database {
    public:
        Database(const char* path);

        Database(const Database&) = delete;

        Database& operator=(const Database&) = delete;

        ~Database();

        void insert_block(glm::ivec3 pos, const char* name);
    private:
        sqlite3* db_;
        sqlite3_stmt* insert_stmt_;
    };
}

#endif // CEPHALON_DATABASE_H_