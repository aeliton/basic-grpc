#include "server/server.h"
#include <proto/example.grpc.pb.h>
#include <proto/example.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

int main()
{
  grpc::ServerBuilder builder;
  builder.AddListeningPort("0.0.0.0:50001", grpc::InsecureServerCredentials());

  CharService charService;
  builder.RegisterService(&charService);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  server->Wait();

  return 0;
}
