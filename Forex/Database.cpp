#include "pch.h"
#include "Database.h"
#include "ForexGlobal.h"

CDatabase::CDatabase(const std::string& server, const std::string& user, const std::string& password)
{
    try
    {
        driver = get_driver_instance();
        pFGlobal.lgr->log(LogLevel::INFO, "Get Driver Instance");
        conn = driver->connect("localhost", "admin", "root");
        pFGlobal.lgr->log(LogLevel::INFO, "Server Connection Successful");
        stml = conn->createStatement();
        pFGlobal.lgr->log(LogLevel::INFO, "Get the statement");
        if (!createDB("FOREX"))
        {
            exit(0);
        }
    }
    catch (sql::SQLException e)
    {
        pFGlobal.lgr->log(LogLevel::EXCEPTION, e.what());
        exit(0);
    }
}

BOOL CDatabase::createDB(const std::string& dbName)
{
    std::string useDbQuery = "USE " + dbName;
    try
    {
        stml->execute(useDbQuery);
        pFGlobal.lgr->log(LogLevel::INFO, "Database connection successful");
        return TRUE;
    }
    catch (sql::SQLException e)
    {
        if (e.getErrorCode() == 1049)
        {
            try
            {
                std::string useCreateQuery = "CREATE DATABASE " + dbName;
                
                stml->execute(useCreateQuery);
                stml->execute(useDbQuery);
                
                pFGlobal.lgr->log(LogLevel::INFO, "Database created and connection successful");
                return TRUE;
            }
            catch (sql::SQLException e)
            {
                pFGlobal.lgr->log(LogLevel::EXCEPTION, e.what());
                return FALSE;
            }
        }
    }

    return FALSE;
}
