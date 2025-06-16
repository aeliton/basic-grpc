#ifndef HELLO_GRPC_SERVER
#define HELLO_GRPC_SERVER

#include <proto/example.grpc.pb.h>
#include <proto/example.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

class CharService : public example::Char::Service
{
public:
  virtual ::grpc::Status Count(
    ::grpc::ServerContext* context, const ::example::Text* text,
    ::example::Number* count
  );
};

#endif
