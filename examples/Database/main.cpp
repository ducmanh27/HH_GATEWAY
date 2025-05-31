#include <QCoreApplication>
#include <exception>
#include <stdio.h>
#include <string>
#include "pqxx/pqxx"
#include "logger/logger.h"
#include "userdao.h"
int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    const std::string dbHost = "127.0.0.1";
    const std::string dbPort = "15432";
    const std::string dbUserName = "postgres";
    const std::string dbPassword = "1";
    const std::string dbName = "vcmt";
    std::string connectInfo = "host=" + dbHost +
                              " port=" + dbPort +
                              " user="  + dbUserName +
                              " password=" + dbPassword +
                              " dbname=" + dbName;
    // C R U D
    printf("Testing connect to db");
    try
    {
        pqxx::connection c(connectInfo);
        if (!c.is_open())
        {
            printf("Falied connect to db");
            return 1;
        }
        //        pqxx::nontransaction ntx {c};
        //        for (auto [username] : ntx.query<std::string>(
        //                 "SELECT username FROM vcmt_user"))
        //        {
        //            VTXLOG_INFO("USER: {}", username);
        //        }
        UserDAO userDAO(c);
        User newUser("ManhPD9", "1");
        userDAO.insertUser(newUser);

    }
    catch (std::exception const& e)
    {
        return 1;
    }

    return a.exec();
}
