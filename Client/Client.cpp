#include "Client.hpp"


Client::Client(/* args */)
{
    clientSocket = 0;
    typeRequest.clear();
    header = false;
    restRequest = "";
    Content_Length = 0;
    readylen = 0;
    boundary = "";
}

Client::~Client()
{
}
