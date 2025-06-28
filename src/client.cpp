#include "client/client.h"

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

#include <iostream>

CharClient::CharClient(const std::string& location)
  : _location(location)
  , _channel(grpc::CreateChannel(_location, grpc::InsecureChannelCredentials()))
  , _status(grpc::Status::OK)
{
}

grpc::Status CharClient::count(const std::string& text, uint32_t& count)
{
  auto stub = example::Char::NewStub(_channel);
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

int main()
{
  std::string input;
  CharClient client("localhost:50001");

  while (true) {
    std::cout << "(q: exit) > ";
    std::cin >> input;
    if (input == "q") {
      break;
    }

    uint32_t count;
    auto status = client.count(input, count);
    if (status.ok()) {
      std::cout << "Client: char count of [" << input << "] is:" << count
                << std::endl;
    } else {
      std::cerr << "Client: Error: the requesst has failed with error code: "
                << status.error_code() << std::endl;
    }
  }
  return 0;
}
