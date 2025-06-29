#include "client/client.h"

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

CharClient::CharClient(std::unique_ptr<example::Char::StubInterface>&& s)
  : _stub(std::move(s))
{
}

CharClient::CharClient(const std::string& location)
  : _stub(example::Char::NewStub(
      grpc::CreateChannel(location, grpc::InsecureChannelCredentials())
    ))
{
}

grpc::Status CharClient::count(const std::string& text, uint32_t& count)
{
  grpc::ClientContext context;
  example::Text input;
  example::Number output;
  input.set_content(text);
  auto status = _stub->Count(&context, input, &output);
  if (status.ok()) {
    count = output.content();
  }
  return status;
}
