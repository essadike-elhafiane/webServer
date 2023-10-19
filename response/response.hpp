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
#include "../Client/Client.hpp"
class response
{
    private:
        /* data */
    public:
        int sendResponse(std::string url, std::string &configResponse, int socketClient, Client & dataClient)
        {
            std::string line;
            std::string response;
            std::cout << url << std::endl;
            if (url == "/Users/eelhafia/Desktop/webServer/y.mp4")
                url = "/Users/eelhafia/Desktop/y.mp4";
            if(dataClient.getdataResponse() == "")
            {
                std::ifstream r(url, std::ios::binary | std::ios::ate);
                if (!r.is_open())
                {
                    std::cout << url << std::endl;
                    std::cerr << "Error open file1"  << std::endl;
                }
                if (url == "/Users/eelhafia/Desktop/y.mp4")
                {
                    std::cout << "|" << r.tellg() << "|" << std::endl;
                    exit(1);
                }
                r.seekg(0);
                
                while (!std::getline(r, line).fail())
                    response = response + line + '\n';
                configResponse = configResponse + std::to_string(response.length()) + "\r\n\r\n" + response;
                dataClient.setdataResponse(configResponse);
            }
            std::cout << "h\n";
            size_t len = write(socketClient, dataClient.getdataResponse().c_str() + dataClient.getLenSend(), dataClient.getdataResponse().length() - dataClient.getLenSend());
            if (len < 0)
            {
                std::cerr << "Failed to send response." << std::endl;
                return 1;
            }
            dataClient.setLenSend(len);
            if (dataClient.getLenSend() < dataClient.getdataResponse().length())
                return 1;
            dataClient.clearLenSend();
            dataClient.resetdataResponse();
            std::cout << "send success" << std::endl;
            return 0;
        }
        response(/* args */);
        ~response();
};


/* code */
#endif //RESPONSE_H
