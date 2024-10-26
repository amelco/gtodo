#ifndef _SQL_API
#define _SQL_API

typedef enum {
    SUCCESS,
    ERROR
} Result;

Result sql_open(const char* conn_string, void* db);
Result sql_close(void* db);

char* sql_get_errormsg();

// Execute a sql query that have a result to be mapped. Ex. SELECT
// Parameters:
//   - the cstring containind the sql query
//   - the pointer to the result (to be mapped by the caller)
// Return: the number of rows returned by the db.
int sql_read(char* query, void* response);

// Execute a sql query that does not have a result. Ex. UPDATE, DELETE
// Return: true on success, otherwise false
bool sql_execute(char* query);

#endif //_SQL_API


#ifdef SQL_API_IMPLEMENTATION
// This implementation uses sqlite3
#include <sqlite3.h>
#include <string.h>

// global to store error messages
char error_msg[255];

Result
sql_open(const char* conn_string, void* db) {
    int result = sqlite3_open(conn_string, db);
    if (result != SQLITE_OK) {
        sprintf(error_msg, "SQL_ERROR: %s", sqlite3_errmsg(db));
        return ERROR;
    }
    return SUCCESS;
}

Result
sql_close(void* db) {
    int result = sqlite3_close(db);
    if (result != SQLITE_OK) {
        sprintf(error_msg, "SQL_ERROR %d: Could not close database properly: %s", result, sqlite3_errmsg(db));
        return ERROR;
    }

}

char*
sql_get_errormsg() {
    return error_msg;
}

#endif
