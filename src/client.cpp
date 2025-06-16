#include "client/client.h"

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

#include <iostream>

int main()
{
  example::Text input;
  example::Number output;

  input.set_content("abaco");

  auto channel =
    grpc::CreateChannel("localhost:50001", grpc::InsecureChannelCredentials());
  std::unique_ptr<example::Char::Stub> stub = example::Char::NewStub(channel);
  grpc::ClientContext context;
  grpc::Status status = stub->Count(&context, input, &output);

  if (status.ok()) {
    std::cout << "Client: char count of [" << input.content()
              << "] is:" << output.content() << std::endl;
  } else {
    std::cout << "Client: Error: the requesst has failed with error code: "
              << status.error_code() << std::endl;
  }

  return 0;
}
