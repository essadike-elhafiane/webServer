#ifndef SOCKETCLIENT_HPP
#define SOCKETCLIENT_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h> 
#include "../server/server.hpp"
#include <set>
#include <fcntl.h>

class Client
{
    private:
        int clientSocket;
        std::string restRequest;
    public:
        Client(/* args */);
        int getClientSocket()
        {
            return clientSocket;
        }
        std::string&  getRestRequest()
        {
            return restRequest;
        }
        void setRestRequest(std::string const& s)
        {
            restRequest = s;
        }
        ~Client();
};



#endif //SOCKET_HPP
