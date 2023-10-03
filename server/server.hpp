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
        int port;
    public:
        server(std::string name);
        std::string getName()
        {
           return name;
        }
        void createSocketServer()
        {
            serverSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (serverSocket < 0) {
                std::cerr << "Error opening socket." << std::endl;
                exit(1);
            }
        }
        void setUpServer()
        {
            std::memset((char*)&serverAddress, 0, sizeof(serverAddress));
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_addr.s_addr = INADDR_ANY; // Bind to any available address
            serverAddress.sin_port = htons(port);
            // bind server;
            if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
                std::cerr << "Error binding socket." << std::endl;
                exit(1);
            }
        }
        int getServerSocket()
        {
            return serverSocket;
        }
        void runServer(int numberClient, int _port)
        {
            port = _port;
            createSocketServer();
            setUpServer();
            if (listen(serverSocket, numberClient) < 0)
            {
                std::cerr << "Failed to listen on socket\n";
                exit(1);
            }
            std::cout << "Server listening on " << port << " ..." << std::endl;
        }
        ~server();
};



/* code */
#endif //SERVER_HPP
