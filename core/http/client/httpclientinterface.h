#ifndef HTTPCLIENTINTERFACE_H
#define HTTPCLIENTINTERFACE_H

#include "json.hpp"
#include "httplib.h"
#include <string>
#include <map>

using HttpResponse = httplib::Result;
using RequestParam = std::map<std::string, std::string>;
using RequestBody = nlohmann::json;

class HttpClientInterface {
    public:
        virtual ~HttpClientInterface() = default;

        virtual HttpResponse get(const std::string &endpoint) = 0;
        virtual HttpResponse getWithParams(const std::string &endpoint, const RequestParam &queryParams) = 0;
        virtual HttpResponse post(const std::string &endpoint, const RequestBody &body) = 0;
        virtual HttpResponse put(const std::string &endpoint, const RequestBody &body) = 0;
        virtual HttpResponse del(const std::string &endpoint) = 0;

        virtual void setBearerToken(const std::string &token) = 0;
        virtual void setDefaultHeaders(const RequestParam &headers) = 0;
        virtual void setTimeout(int seconds) = 0;
};

#endif // HTTPCLIENTINTERFACE_H
