#include "userdao.h"

#include "logger/logger.h"
#include <stdio.h>
User::User(int id, const std::string& userName, const std::string& password) : mId(id), mUserName(userName),
    mPassword(password)
{

}

User::User(const std::string& userName, const std::string& password) : mUserName(userName),
    mPassword(password)
{

}

int User::id() const
{
    return mId;
}

void User::setId(int id)
{
    mId = id;
}

std::string User::userName() const
{
    return mUserName;
}

void User::setUserName(const std::string& userName)
{
    mUserName = userName;
}

std::string User::password() const
{
    return mPassword;
}

void User::setPassword(const std::string& password)
{
    mPassword = password;
}


UserDAO::UserDAO(pqxx::connection& c) : mConn(c)
{

}

void UserDAO::insertUser(const User& user)
{
    try
    {
        pqxx::work txn(mConn);
        pqxx::result result = txn.exec_params("INSERT INTO vcmt_user (username, password) VALUES ($1, $2)",
                                              user.userName(),
                                              user.password());
        txn.commit();

    }
    catch (std::exception const& e)
    {
        printf("%s", e.what());
    }
}
