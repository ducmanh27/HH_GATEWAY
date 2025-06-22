#include "httpclientimpl.h"
#include <sstream>

HttpClientImpl::HttpClientImpl(const std::string &baseUrl, bool useHttps)
    : m_baseUrl(baseUrl), m_isHttps(useHttps) {}

HttpResponse HttpClientImpl::get(const std::string &endpoint) {
    if (m_isHttps) {
        #ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        auto client = createSSLClient();
        return client->Get(endpoint.c_str(), buildHeaders());
        #else
        throw std::runtime_error("HTTPS requested but SSL not supported");
        #endif
    }
    else {
        auto client = createClient();
        return client->Get(endpoint.c_str(), buildHeaders());
    }
}

HttpResponse HttpClientImpl::getWithParams(const std::string &endpoint, const RequestParam &queryParams) {
    std::string fullEndpoint = appendQueryParams(endpoint, queryParams);
    if (m_isHttps) {
        #ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        auto client = createSSLClient();
        return client->Get(fullEndpoint.c_str(), buildHeaders());
        #else
        throw std::runtime_error("HTTPS requested but SSL not supported");
        #endif
    }
    else {
        auto client = createClient();
        return client->Get(fullEndpoint.c_str(), buildHeaders());
    }
}

HttpResponse HttpClientImpl::post(const std::string &endpoint, const RequestBody &body) {
    if (m_isHttps) {
        #ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        auto client = createSSLClient();
        return client->Post(endpoint.c_str(), buildHeaders(), body.dump(), "application/json");
        #else
        throw std::runtime_error("HTTPS requested but SSL not supported");
        #endif
    }
    else {
        auto client = createClient();
        return client->Post(endpoint.c_str(), buildHeaders(), body.dump(), "application/json");
    }
}

HttpResponse HttpClientImpl::put(const std::string &endpoint, const RequestBody &body) {
    if (m_isHttps) {
        #ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        auto client = createSSLClient();
        return client->Put(endpoint.c_str(), buildHeaders(), body.dump(), "application/json");
        #else
        throw std::runtime_error("HTTPS requested but SSL not supported");
        #endif
    }
    else {
        auto client = createClient();
        return client->Put(endpoint.c_str(), buildHeaders(), body.dump(), "application/json");
    }
}

HttpResponse HttpClientImpl::del(const std::string &endpoint) {
    if (m_isHttps) {
        #ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        auto client = createSSLClient();
        return client->Delete(endpoint.c_str(), buildHeaders());
        #else
        throw std::runtime_error("HTTPS requested but SSL not supported");
        #endif
    }
    else {
        auto client = createClient();
        return client->Delete(endpoint.c_str(), buildHeaders());
    }
}

void HttpClientImpl::setBearerToken(const std::string &token) {
    m_bearerToken = token;
}

void HttpClientImpl::setDefaultHeaders(const RequestParam &headers) {
    m_defaultHeaders = headers;
}

void HttpClientImpl::setTimeout(int seconds) {
    m_timeout = seconds;
}

httplib::Headers HttpClientImpl::buildHeaders() {
    httplib::Headers headers;
    for (const auto& [key, value] : m_defaultHeaders) {
        headers.emplace(key, value);
    }
    if (!m_bearerToken.empty()) {
        headers.emplace("Authorization", "Bearer " + m_bearerToken);
    }
    return headers;
}

bool HttpClientImpl::isHttps() const
{
    return m_isHttps;
}

void HttpClientImpl::setIsHttps(bool newIsHttps)
{
    m_isHttps = newIsHttps;
}

std::string HttpClientImpl::baseUrl() const {
    return m_baseUrl;
}

void HttpClientImpl::setBaseUrl(const std::string &newBaseUrl) {
    m_baseUrl = newBaseUrl;
}

std::string HttpClientImpl::appendQueryParams(const std::string &endpoint, const RequestParam &params) {
    if (params.empty()) return endpoint;

    std::ostringstream oss;
    oss << endpoint << "?";
    bool first = true;
    for (const auto& [key, value] : params) {
        if (!first) oss << "&";
        oss << key << "=" << value;
        first = false;
    }
    return oss.str();
}

std::unique_ptr<httplib::Client> HttpClientImpl::createClient() {
    auto client = std::make_unique<httplib::Client>(m_baseUrl);
    client->set_connection_timeout(m_timeout);
    return client;
}

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
std::unique_ptr<httplib::SSLClient> HttpClientImpl::createSSLClient() {
    auto client = std::make_unique<httplib::SSLClient>(m_baseUrl);
    client->enable_server_certificate_verification(false);  // Hoặc true nếu cần xác minh chứng chỉ
    client->set_connection_timeout(m_timeout);
    return client;
}
#endif
