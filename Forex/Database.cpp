#include "pch.h"
#include "Database.h"
#include "ForexGlobal.h"

CDatabase::CDatabase(const std::string& server, const std::string& user, const std::string& password)
{
    try
    {
        driver = get_driver_instance();
        conn = driver->connect("localhost", "admin", "root");
        pFGlobal.lgr->log(LogLevel::INFO, "DATABASE connection successful!");
    }
    catch (sql::SQLException e)
    {
        pFGlobal.lgr->log(LogLevel::EXCEPTION, e.what());
        exit(0);
    }
}
