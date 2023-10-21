#ifndef SERVER_HPP
#define SERVER_HPP

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

class server
{
    private: 
        std::string name;
        int serverSocket;
        struct sockaddr_in serverAddress;
    public:
        int port;
        server(std::string name);
        std::string getName()
        {
           return name;
        }
        int createSocketServer()
        {
            serverSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (serverSocket < 0) {
                std::cerr << "Error opening socket." << std::endl;
                return 1;
            }
            return 0;
        }
        int setUpServer()
        {
            std::memset((char*)&serverAddress, 0, sizeof(serverAddress));
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_addr.s_addr = INADDR_ANY; // Bind to any available address
            serverAddress.sin_port = htons(port);
            // bind server;
            
            int opt = 1;
            if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
                std::cerr << "Failed to set socket options." << std::endl;
                return 1;
            }

            if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
                std::cerr << "Error binding socket." << std::endl;
                return 1;
            }
            return 0;
        }
        int getServerSocket()
        {
            return serverSocket;
        }
        int runServer(int numberClient, int _port)
        {
            port = _port;
            if (createSocketServer())
                return 1;
            if (setUpServer())
            {
                close(serverSocket);
                return 1;
            }
            if (listen(serverSocket, numberClient) < 0)
            {
                std::cerr << "Failed to listen on socket\n";
                return 1;
            }
            
            std::cout << "Server " << name <<" listening on " << port << " ..." << std::endl;
            return 0;
        }
        ~server();
};



/* code */
#endif //SERVER_HPP
