#include "pch.h"
#include "Database.h"
#include "ForexGlobal.h"

CDatabase::CDatabase(const std::string& server, const std::string& user, const std::string& password)
{
    dbName = "FOREX";
    try
    {
        driver = get_driver_instance();
        pFGlobal.lgr->log(LogLevel::INFO, "Get Driver Instance");
        conn = driver->connect("localhost", "admin", "root");
        pFGlobal.lgr->log(LogLevel::INFO, "Server Connection Successful");
        stml = conn->createStatement();
        pFGlobal.lgr->log(LogLevel::INFO, "Get the statement");
        if (!createDB(dbName))
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

BOOL CDatabase::checkTableExist(const std::string& tableName)
{
    std::string checkTableQuery =
        "SELECT COUNT(*) FROM information_schema.tables "
        "WHERE table_schema = '" + dbName + "' AND table_name = '" + tableName + "'";
    try
    {
        std::unique_ptr<sql::ResultSet>res(stml->executeQuery(checkTableQuery));
        res->next();
        bool tableExists = res->getInt(1) > 0;
        if (tableExists)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    catch (sql::SQLException e)
    {
        pFGlobal.lgr->log(LogLevel::EXCEPTION, "Check Table Exist Failed!");
        exit(0);
    }
    return FALSE;
}

BOOL CDatabase::createTable(const std::string& createTableQuery)
{
    try
    {
        stml->execute(createTableQuery);
        pFGlobal.lgr->log(LogLevel::INFO, createTableQuery + " Query Successful");
    }
    catch (sql::SQLException e)
    {
        pFGlobal.lgr->log(LogLevel::EXCEPTION, "Create Table Failed!");
    }
}
