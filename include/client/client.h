#ifndef HELLO_GRPC_CLIENT
#define HELLO_GRPC_CLIENT

#include <proto/example.grpc.pb.h>
#include <proto/example.pb.h>

class CharClient
{
public:
  explicit CharClient(const std::string& location);
  grpc::Status count(const std::string& text, uint32_t& count);

private:
  std::string _location;
  std::shared_ptr<grpc::Channel> _channel;
  grpc::Status _status;
};

#endif
