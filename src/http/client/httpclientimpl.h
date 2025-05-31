#ifndef HTTPCLIENTIMPL_H
#define HTTPCLIENTIMPL_H

#include "httpclientinterface.h"
#include <string>
class HttpClientImpl : public HttpClientInterface
{
public:
    HttpClientImpl();

    // HttpClientInterface interface
public:
    HttpResponse requestHttpGET(Url& url, Endpoint endpoint) override;
    HttpResponse requestHttpGET(Url& url, Endpoint endpoint, RequestParam param) override;
    HttpResponse requestHttpPOST(Url& url, Endpoint endpoint, RequestBody body) override;
    Endpoint getEndpointWithRequestParam(Endpoint endpoint, RequestParam param);
};

#endif // HTTPCLIENTIMPL_H
