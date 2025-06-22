#ifndef HTTPCLIENTIMPL_H
#define HTTPCLIENTIMPL_H

#include "httpclientinterface.h"
#include <memory>

class HttpClientImpl : public HttpClientInterface {
    public:
        explicit HttpClientImpl(const std::string &baseUrl, bool useHttps = false);

        HttpResponse get(const std::string &endpoint) override;
        HttpResponse getWithParams(const std::string &endpoint, const RequestParam &queryParams) override;
        HttpResponse post(const std::string &endpoint, const RequestBody &body) override;
        HttpResponse put(const std::string &endpoint, const RequestBody &body) override;
        HttpResponse del(const std::string &endpoint) override;

        void setBearerToken(const std::string &token) override;
        void setDefaultHeaders(const RequestParam &headers) override;
        void setTimeout(int seconds) override;

        std::string appendQueryParams(const std::string &endpoint, const RequestParam &params);
        std::string baseUrl() const;
        void setBaseUrl(const std::string &newBaseUrl);

        bool isHttps() const;
        void setIsHttps(bool newIsHttps);

    private:
        std::unique_ptr<httplib::Client> createClient();
        #ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        std::unique_ptr<httplib::SSLClient> createSSLClient();
        #endif
        httplib::Headers buildHeaders();

        std::string m_baseUrl;
        bool m_isHttps = false;
        RequestParam m_defaultHeaders;
        std::string m_bearerToken;
        int m_timeout = 1;
};

#endif // HTTPCLIENTIMPL_H
