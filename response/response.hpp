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
            std::cout << "|" << url << std::endl;
            if (url == "/Users/eelhafia/Desktop/webServer/y.mp4")
                url = "/Users/eelhafia/Desktop/y.mp4";
            if (url == "/Users/eelhafia/Desktop/webServer/Users/eelhafia/goinfre/send/u.mp4")
                url = "/Users/eelhafia/goinfre/send/u.mp4";
            if (url == "/Users/eelhafia/Desktop/webServer/goinfre/m.mp4")
                url = "/goinfre/m.mp4";
            if(!dataClient.getLenSend())
            {
                std::ifstream r(url, std::ios::binary | std::ios::ate);
                if (!r.is_open())
                {
                    std::cout << url << std::endl;
                    std::cerr << "Error open file1"  << std::endl;
                }
                // if (url == "/Users/eelhafia/Desktop/y.mp4")
                // {
                //     std::cout << "|" <<   "|" << std::endl;
                //     exit(1);
                // }
                configResponse = configResponse + std::to_string(r.tellg()) + "\r\n\r\n";
                dataClient.lengthFile = static_cast<size_t>(r.tellg());
                std::cout << "dfhd\n";
                send(socketClient, configResponse.c_str() , configResponse.length() , 0);
                r.close();
                // r.seekg(0);
                // dataClient.setdataResponse(configResponse);

            }
            
            std::ifstream input(url, std::ios::binary);
            char buffer[30001];
            // memset(buffer, 0 ,3001);
            if(!input.is_open())
            {
                std::cout << "error on file" << url << std::endl;
            }
            input.seekg(dataClient.getLenSend());
            size_t lenRead = 30000;
            if (dataClient.lengthFile - dataClient.getLenSend() < 30000)
                lenRead = dataClient.lengthFile - dataClient.getLenSend();
            input.read(buffer, lenRead);
            if ((input.tellg() <= 0))
            {
                std::cout << "fhd\n" << input.tellg() << "|" << dataClient.getLenSend()  << "|" << dataClient.lengthFile << std::endl;
                input.close();
                dataClient.clearLenSend();
                return 0;
            }
            std::cout << "|"<<static_cast<ssize_t>(input.tellg()) - dataClient.getLenSend() << "h\n";
            size_t len = send(socketClient, buffer , input.tellg() - dataClient.getLenSend(), 0);
            if (len < 0)
            {
                std::cerr << "Failed to send response." << std::endl;
                return 1;
            }
            dataClient.setLenSend(len);
            if (dataClient.getLenSend() < dataClient.lengthFile)
                return 1;
            input.close();
            dataClient.clearLenSend();
            // dataClient.resetdataResponse();
            std::cout << "send success" << std::endl;
            return 0;
        }
        response(/* args */);
        ~response();
};


/* code */
#endif //RESPONSE_H
