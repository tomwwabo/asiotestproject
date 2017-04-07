#include "server.h"
#include "connection.h"

#include "asio.hpp"

using namespace asio;

int main(int argc, char *argv[])
{
    std::shared_ptr<Server> s = std::shared_ptr<Server>(new Server(4711)); //listen on port 4711
    s->run();
}
