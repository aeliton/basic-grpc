#include "client/client.h"

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

CharClient::CharClient() = default;

CharClient::CharClient(const std::string& location)
  : _location(location)
  , _channel(grpc::CreateChannel(_location, grpc::InsecureChannelCredentials()))
  , _status(grpc::Status::OK)
{
}

grpc::Status CharClient::count(const std::string& text, uint32_t& count)
{
  auto stub = createStub();
  grpc::ClientContext context;
  example::Text input;
  example::Number output;
  input.set_content(text);
  auto status = stub->Count(&context, input, &output);
  if (status.ok()) {
    count = output.content();
  }
  return status;
}

std::unique_ptr<example::Char::StubInterface> CharClient::createStub()
{
  return example::Char::NewStub(_channel);
}
