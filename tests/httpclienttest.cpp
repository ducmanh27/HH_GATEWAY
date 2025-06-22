#include "httpclienttest.h"
#include "json.hpp"

TEST_F(HttpClientTest, AppendQueryParams_CorrectlyFormatsUrl) {
    std::string endpoint = "/v1/api/test";
    RequestParam params = {
        {"day", "19"},
        {"month", "5"},
        {"year", "2025"}
    };

    std::string fullPath = client->appendQueryParams(endpoint, params);
    EXPECT_TRUE(fullPath.find("day=19") != std::string::npos);
    EXPECT_TRUE(fullPath.find("month=5") != std::string::npos);
    EXPECT_TRUE(fullPath.find("year=2025") != std::string::npos);
    EXPECT_TRUE(fullPath.find("/v1/api/test?") == 0);
}

TEST_F(HttpClientTest, GetWithoutParams) {
    auto res = client->get(mockGetEndpoint);
    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 200);
}

TEST_F(HttpClientTest, GetWithParams) {
    RequestParam params = {
        {"day", "19"},
        {"month", "6"}
    };
    auto res = client->getWithParams(mockGetEndpoint, params);
    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 200);
    EXPECT_NE(res->body.find("day"), std::string::npos);
    EXPECT_NE(res->body.find("month"), std::string::npos);
}

TEST_F(HttpClientTest, PostJsonBody) {
    RequestBody body = {
        {"name", "ChatGPT"},
        {"type", "AI"}
    };
    auto res = client->post(mockPostEndpoint, body);
    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 200);
    EXPECT_NE(res->body.find("ChatGPT"), std::string::npos);
}

TEST_F(HttpClientTest, PutJsonBody) {
    RequestBody body = {
        {"key", "value"}
    };
    auto res = client->put(mockPutEndpoint, body);
    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 200);
    EXPECT_NE(res->body.find("key"), std::string::npos);
}

TEST_F(HttpClientTest, DeleteRequest) {
    auto res = client->del(mockDeleteEndpoint);
    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 200);
}

TEST_F(HttpClientTest, SetBearerToken) {
    client->setBearerToken("12345TOKEN");

    RequestBody body = {
        {"test", "bearer"}
    };
    auto res = client->post(mockPostEndpoint, body);

    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 200);
    EXPECT_NE(res->body.find("Bearer"), std::string::npos);  // httpbin.org/post sẽ phản hồi lại header
}

TEST_F(HttpClientTest, SetDefaultHeaders) {
    RequestParam headers = {
        {"X-Custom-Header", "Hello"}
    };
    client->setDefaultHeaders(headers);

    auto res = client->get(mockGetEndpoint);
    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 200);
    EXPECT_NE(res->body.find("X-Custom-Header"), std::string::npos);
}

TEST_F(HttpClientTest, AppendQueryParams_Utility) {
    RequestParam params = {
        {"year", "2025"},
        {"lang", "cpp"}
    };

    std::string full = client->appendQueryParams("/api/test", params);
    EXPECT_TRUE(full.find("year=2025") != std::string::npos);
    EXPECT_TRUE(full.find("lang=cpp") != std::string::npos);
    EXPECT_TRUE(full.find("/api/test?") == 0);
}

TEST_F(HttpClientTest, GetWithSSL) {
    #ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    // Tạo client HTTPS riêng
    HttpClientImpl httpsClient("httpbin.org", true);
    httpsClient.setTimeout(3);

    auto res = httpsClient.get("/get");
    ASSERT_TRUE(res);
    EXPECT_EQ(res->status, 200);
    EXPECT_NE(res->body.find("url"), std::string::npos); // httpbin trả về URL
    #else
    GTEST_SKIP() << "SSL not supported. Define CPPHTTPLIB_OPENSSL_SUPPORT to enable this test.";
    #endif
}
