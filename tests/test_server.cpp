#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <proto/example_mock.grpc.pb.h>

#include "client/client.h"

using namespace example;
using namespace ::testing;
using namespace grpc;

class TestClient : public CharClient
{
public:
  std::unique_ptr<example::Char::StubInterface> createStub() override
  {
    Number resp;
    resp.set_content(4);
    auto stub = std::make_unique<MockCharStub>();
    EXPECT_CALL(*stub, Count(_, _, _))
      .Times(AtLeast(1))
      .WillOnce(DoAll(SetArgPointee<2>(resp), Return(Status::OK)));
    return stub;
  }
};

TEST(gRpcServerTest, StartsWithNoConnections)
{
  TestClient client;
  uint32_t count;
  auto status = client.count("test", count);
  EXPECT_TRUE(status.ok());
  EXPECT_EQ(count, 4);
}
