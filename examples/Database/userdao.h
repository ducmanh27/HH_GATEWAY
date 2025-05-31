#ifndef USERDAO_H
#define USERDAO_H
#include <string>
#include "pqxx/pqxx"
class User
{
public:
    User(int id, const std::string& userName, const std::string& password);
    User(const std::string& userName, const std::string& password);
    int id() const;
    void setId(int id);

    std::string userName() const;
    void setUserName(const std::string& userName);


    std::string password() const;
    void setPassword(const std::string& password);

private:
    int mId {0};
    std::string mUserName {""};
    std::string mPassword {""};

};

class UserDAO
{
public:
    UserDAO(pqxx::connection& c);
    void insertUser(const User& user);

private:
    pqxx::connection& mConn;
};

#endif // USERDAO_H
