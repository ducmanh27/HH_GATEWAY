#include "httpclientimpl.h"
#include "iostream"
HttpClientImpl::HttpClientImpl() {}

HttpResponse HttpClientImpl::requestHttpGET(Url &url, Endpoint endpoint) {
  Client client(url);
  client.set_connection_timeout(1);
  return client.Get(endpoint);
}

HttpResponse HttpClientImpl::requestHttpGET(Url &url, Endpoint endpoint,
                                            RequestParam param) {
  Client client(url);
  if (!param.empty()) {
    endpoint = getEndpointWithRequestParam(endpoint, param);
  }
  client.set_connection_timeout(1);
  return client.Get(endpoint);
}

HttpResponse HttpClientImpl::requestHttpPOST(Url &url, Endpoint endpoint,
                                             RequestBody body) {
  Client client(url);
  client.set_connection_timeout(1);
  return client.Post(endpoint.c_str(), body.dump(), "application/json");
}

Endpoint HttpClientImpl::getEndpointWithRequestParam(Endpoint endpoint,
                                                     RequestParam param) {
  endpoint += "?";
  bool first = true;
  for (const auto &[key, value] : param) {
    if (!first) {
      endpoint += "&";
    }
    endpoint += key + "=" + value;
    first = false;
  }
  return endpoint;
}
