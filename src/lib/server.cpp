#include "server/server.h"
#include <proto/example.grpc.pb.h>
#include <proto/example.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

::grpc::Status CharService::Count(
  [[maybe_unused]] ::grpc::ServerContext* context, const ::example::Text* text,
  ::example::Number* count
)
{
  std::cout << "Server: request for: [" << text->content() << "]" << std::endl;
  count->set_content(text->content().size());
  return grpc::Status::OK;
}
