#include "client/client.h"

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

#include <iostream>

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
