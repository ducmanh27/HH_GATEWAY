#ifndef HTTPCLIENTINTERFACE_H
#define HTTPCLIENTINTERFACE_H

#include "json.hpp"
#include "httplib.h"
using namespace httplib;
using HttpResponse = httplib::Result;
using Url = std::string;
using RequestParam = std::map<std::string, std::string>;
using RequestBody = nlohmann::json;
using Endpoint = std::string;
class HttpClientInterface
{
public:
    virtual ~HttpClientInterface() = default;
    virtual HttpResponse requestHttpGET(Url& url , Endpoint endpoint) = 0;
    virtual HttpResponse requestHttpGET(Url& url, Endpoint endpoint, RequestParam param) = 0;
    virtual HttpResponse requestHttpPOST(Url& url, Endpoint endpoint, RequestBody body) = 0;
};

#endif // HTTPCLIENTINTERFACE_H
