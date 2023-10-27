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

        void sendStringResponse(Client &dataClient)
        {
            size_t len = send(dataClient.getClientSocket(), dataClient.getCgi().c_str() , dataClient.getCgi().size(), 0);
            if (len < 0)
            {
                std::cerr << "Failed to send response." << std::endl;
                return ;
            }
            dataClient.SetCgi("");
        }

        void headre(Client & dataClient , std::string &url , std::string & configResponse)
        {
            std::cout << dataClient.error << std::endl;
            std::map<int , std::string>::const_iterator it = dataClient.configData.error_page.begin();
            while(it != dataClient.configData.error_page.end())
            {
                if(it->first == dataClient.error)
                    break;
                it++;
            }

            if (dataClient.error == 0)
            {
                configResponse = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: ";
                return;
            }
            else if (dataClient.error == 400 && it != dataClient.configData.error_page.end())
            {
                url = dataClient.configData.error_page[400];
                configResponse = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: ";
                return;
            }
            else if (dataClient.error == 401  && it != dataClient.configData.error_page.end())
            {
                url = dataClient.configData.error_page[401];
                configResponse = "HTTP/1.1 401 Unauthorized\r\nConnection: close\r\nContent-Length: ";
                return;
            }
            else if (dataClient.error == 403 && it != dataClient.configData.error_page.end())
            {
                std::cout << dataClient.configData.error_page[403]<< "fhd\n" ;
                url = dataClient.configData.error_page[403];
                configResponse = "HTTP/1.1 403 Forbidden\r\nConnection: close\r\nContent-Length: ";
                return;
            }
            else if (dataClient.error == 404 && it != dataClient.configData.error_page.end())
            {
                url = dataClient.configData.error_page[404];
                configResponse = "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Length: ";
                return;
            }
            else if (dataClient.error == 413 && it != dataClient.configData.error_page.end())
            {
                url = dataClient.configData.error_page[413];
                configResponse = "HTTP/1.1 502 Bad Gateway\r\nConnection: close\r\nContent-Length: ";
                return;
            }
            else
            {
                url = dataClient.configData.error_page.begin()->second;
                configResponse = "HTTP/1.1 500 Internal Server Error\r\nConnection: close\r\nContent-Length: ";
            }
            std::cout << url << std::endl;
        }

        int sendResponse(Client & dataClient)
        {
            std::string configResponse;
            std::string line;
            std::string url;
            std::string response;
            // std::cout << "|" << dataClient.getUrl() << std::endl;
            url = dataClient.getUrl();
            std::cout << "---|" <<  url << "|" << dataClient.getClientSocket() << "|---"<< std::endl;
            
            // if (url == "/Users/mserrouk/Desktop/webServer/Users/mserrouk/goinfre/send/u.mp4")
            //     url = "/Users/mserrouk/goinfre/send/u.mp4";
            if(!dataClient.getLenSend())
            {
                headre(dataClient , url , configResponse);
                std::cout << "---|" <<  url << "|" << dataClient.getClientSocket() << "|" << dataClient.error << "|---"<< std::endl;
                std::ifstream r(url, std::ios::binary | std::ios::ate);
                if (!r.is_open())
                {
                    std::cout << url << std::endl;
                    std::cerr << "Error open file1"  << std::endl;
                    return 0;
                }
                configResponse = configResponse + std::to_string(r.tellg()) + "\r\n\r\n";
                dataClient.lengthFile = static_cast<size_t>(r.tellg());
                size_t len = send(dataClient.getClientSocket(), configResponse.c_str() , configResponse.length() , 0);
                if (len < 0)
                {
                    std::cerr << "Failed to send response." << std::endl;
                    return 0;
                }
                r.close();
            }   
            std::cout << dataClient.getLenSend() <<  "mm\n";   
            std::ifstream input(url, std::ios::binary);
            char buffer[1000001];
            if(!input.is_open())
            {
                std::cout << "error on file" << url << std::endl;
                return 0;
            }
            input.seekg(dataClient.getLenSend());
            size_t lenRead = 1000000;
            if (dataClient.lengthFile - dataClient.getLenSend() < 1000000)
                lenRead = dataClient.lengthFile - dataClient.getLenSend();
            input.read(buffer, lenRead);
            if ((input.tellg() <= 0))
            {
                // std::cout << "fhd\n" << input.tellg() << "|" << dataClient.getLenSend()  << "|" << dataClient.lengthFile << std::endl;
                input.close();
                dataClient.clearLenSend();
                return 0;
            }
            std::cout << dataClient.getClientSocket() << "|"<<static_cast<ssize_t>(input.tellg()) - dataClient.getLenSend() << "h\n";
            std::cout << "ttt\n";
            size_t len = send(dataClient.getClientSocket(), buffer , input.tellg() - dataClient.getLenSend(), 0);
            std::cout << "tttp\n";

            if (len < 0)
            {
                std::cerr << "Failed to send response." << std::endl;
                return 1;
            }
            std::cout << "ss\n";
            dataClient.setLenSend(len);
            input.close();
            if (dataClient.getLenSend() < dataClient.lengthFile)
                return 1;
            dataClient.clearLenSend();
            // dataClient.resetdataResponse();
            std::cout << "|" << url  << "|"<< std::endl;
            std::cout << "send success" << std::endl;
            return 0;
        }
        response(/* args */);
        ~response();
};

/* code */
#endif //RESPONSE_H
