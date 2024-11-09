#pragma once
//DATABASE HEADERS/////////////////////
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
///////////////////////////////////////

class CDatabase
{
private:
	sql::Driver* driver;
	sql::Connection* conn;
	sql::Statement* stml;

public:
	CDatabase(const std::string& server, const std::string& user, const std::string& password);
	BOOL createDB(const std::string& dbName);
};

