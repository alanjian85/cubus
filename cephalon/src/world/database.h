#ifndef CEPHALON_DATABASE_H_
#define CEPHALON_DATABASE_H_

#include <sqlite3.h>

namespace cephalon {
    class Database {
    public:
        Database(const char* path);

        Database(const Database&) = delete;

        Database& operator=(const Database&) = delete;

        ~Database();
    private:
        sqlite3* db_;
    };
}

#endif // CEPHALON_DATABASE_H_