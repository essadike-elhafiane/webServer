#include "server.hpp"

server::server(std::string name, std::string host)
{
    port = 3000;
    this->host = host;
    this->name = name;
}

server::~server()
{
}
