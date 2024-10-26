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
Result sql_read(void* db, char* query, void* response);

// Execute a sql query that does not have a result. Ex. UPDATE, DELETE
// Return: true on success, otherwise false
bool sql_execute(char* query);

#endif //_SQL_API


#ifdef SQL_API_IMPLEMENTATION
// This implementation uses sqlite3
#include <sqlite3.h>
#include <string.h>

// globals
char error_msg[255];
//sqlite3* db;
char*
sql_get_errormsg() {
    return error_msg;
}

Result
sql_open(const char* conn_string, void* _db) {
    int result = sqlite3_open(conn_string, _db);
    if (result != SQLITE_OK) {
        sprintf(error_msg, "SQL_ERROR: Could not open connection: %s", sqlite3_errmsg(_db));
        return ERROR;
    }
    //db = _db;
    return SUCCESS;
}

Result
sql_close(void* db) {
    int result = sqlite3_close(db);
    if (result != SQLITE_OK) {
        sprintf(error_msg, "SQL_ERROR %d: Could not close connection: %s", result, sqlite3_errmsg(db));
        return ERROR;
    }
    return SUCCESS;
}


Result
sql_read(void* db, char* query, void* response) {
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        sprintf(error_msg, "SQL_ERROR: Could not prepare statement: %s", sqlite3_errmsg(db));
        return ERROR;
    }

    int ret_code = 0;
    while((ret_code = sqlite3_step(stmt)) == SQLITE_ROW) {
        TraceLog(LOG_INFO, "TEST: ID = %d\n", sqlite3_column_int(stmt, 0));
        // TODO: find a way to generically map the result rows.
        // Maybe using a function pointer that handles the mapping passed by the caller?
    }
    if(ret_code != SQLITE_DONE) {
        sprintf(error_msg, "SQL_ERROR: while performing sql: %s\n", sqlite3_errmsg(db));
        return ERROR;
    }

    sqlite3_finalize(stmt);
    return SUCCESS;
}

#endif
