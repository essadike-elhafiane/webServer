#include "server.hpp"

server::server(std::string name)
{
    port = 8080;
    this->name = name;
}

server::~server()
{
}
