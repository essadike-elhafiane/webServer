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
        void sendResponse(std::string url, std::string &configResponse, int socketClient)
        {
            std::string line;
            std::string response;
            // std::cout << url << std::endl;
            // if (url == "/Users/eelhafia/Desktop/webServer/y.mp4")
            //     url = "/Users/eelhafia/Desktop/y.mp4";
            std::ifstream r(url, std::ios::binary);
            if (!r.is_open())
            {
                std::cout << url << std::endl;
                std::cerr << "Error open file1"  << std::endl;
                // exit(1);
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
            // std::cout << socketClient << std::endl;
            return;
        }
        response(/* args */);
        ~response();
};


/* code */
#endif //RESPONSE_H
