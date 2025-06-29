#ifndef HELLO_GRPC_CLIENT
#define HELLO_GRPC_CLIENT

#include <proto/example.grpc.pb.h>
#include <proto/example.pb.h>

class CharClient
{
public:
  explicit CharClient();
  explicit CharClient(const std::string& location);
  grpc::Status count(const std::string& text, uint32_t& count);
  virtual std::unique_ptr<example::Char::StubInterface> createStub();

private:
  std::string _location;
  std::shared_ptr<grpc::Channel> _channel;
  grpc::Status _status;
};

#endif
