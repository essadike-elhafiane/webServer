#ifndef RESPONSE_H
#define RESPONSE_H

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
#include <fcntl.h>

class response
{
    private:
        /* data */
    public:
        void sendResponse(std::string const &url, std::string &configResponse, int socketClient)
        {
            std::string line;
            std::string response;
            std::ifstream r(url);
            if (!r.is_open())
            {
                std::cout << url << std::endl;
                std::cerr << "Error open file1"  << std::endl;
                exit(1);
            }
            while (!std::getline(r, line).fail())
                response = response + line + '\n';
                configResponse = configResponse + std::to_string(response.length()) + "\r\n\r\n" + response;
            if (send(socketClient, configResponse.c_str(), configResponse.length(), 0) < 0)
            {
                std::cerr << "Failed to send response." << std::endl;
                close(socketClient);
                return;
            }
            return;
        }
        response(/* args */);
        ~response();
};


/* code */
#endif //RESPONSE_H
