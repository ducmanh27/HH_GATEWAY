#include "httpclienttest.h"
#include "http/client/httpclientimpl.h"

HttpClientTest::HttpClientTest()
{

}

TEST_F(HttpClientTest, getEndpointWithParam)
{
    HttpClientImpl client;
    Endpoint endpoint = "/v1/api/test";
    RequestParam param;
    param["day"] = "19";
    param["month"] = "5";
    param["year"] = "2025";
    Endpoint newEndPointWithParam = client.getEndpointWithRequestParam(endpoint, param);
    EXPECT_EQ(newEndPointWithParam, "/v1/api/test?day=19&month=5&year=2025");
}
