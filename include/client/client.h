#ifndef HELLO_GRPC_CLIENT
#define HELLO_GRPC_CLIENT

#include <proto/example.grpc.pb.h>
#include <proto/example.pb.h>

class CharClient
{
public:
  CharClient(std::unique_ptr<example::Char::StubInterface>&& s);
  explicit CharClient(const std::string& location);
  grpc::Status count(const std::string& text, uint32_t& count);

private:
  std::unique_ptr<example::Char::StubInterface> _stub;
};

#endif
