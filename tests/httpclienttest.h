#ifndef HTTPCLIENTTEST_H
#define HTTPCLIENTTEST_H

#include "http/client/httpclientimpl.h"
#include <gtest/gtest.h>

class HttpClientTest : public ::testing::Test {
    protected:
        void SetUp() override {
            client = std::make_unique<HttpClientImpl>(mockUrl);
        }

        void TearDown() override {}

        std::unique_ptr<HttpClientImpl> client;
        std::string mockUrl = "http://httpbin.org";

        std::string mockPostEndpoint = "/post";
        std::string mockGetEndpoint = "/get";
        std::string mockPutEndpoint = "/put";
        std::string mockDeleteEndpoint = "/delete";
};

#endif // HTTPCLIENTTEST_H
