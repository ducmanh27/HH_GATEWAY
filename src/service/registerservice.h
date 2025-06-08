#ifndef REGISTERSERVICE_H
#define REGISTERSERVICE_H
#include <map>
#include <string>
#include "json.hpp"

using Reply = std::pair<std::string, std::string>;
class RegisterService {
    public:
        RegisterService();
        Reply processRegisterMessage(const std::string &message);
    private:
        std::map<std::string, int> mMacToNodeId;
        int mNextNodeId = 1;

};

#endif // REGISTERSERVICE_H
