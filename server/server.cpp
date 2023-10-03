#include "server.hpp"

server::server(std::string name)
{
    port = 3000;
    this->name = name;
}

server::~server()
{
}
