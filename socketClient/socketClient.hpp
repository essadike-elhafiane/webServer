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
class socketClient
{
    private:
        socklen_t clientLength;
        int  clientSocket;
        struct sockaddr_in clientAddress;
    public:
        socketClient(/* args */);
        int getClientSocket()
        {
            return clientSocket;
        }
        int setUpAndAccept(server &server)
        {
            clientLength = sizeof(clientAddress);
            clientSocket = accept(server.getServerSocket(), (struct sockaddr*)&clientAddress, &clientLength);
            if (clientSocket < 0) {
                std::cerr << "Error accepting client connection." << std::endl;
                exit(1);
            }
            fcntl(clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
            return clientSocket;
        }
    
        ~socketClient();
};



#endif //SOCKET_HPP
