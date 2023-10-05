#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include "../socketClient/socketClient.hpp"
#include <sstream>
#include <vector>
#include <cstring>

class request
{
    private:
        std::string url;
        int len;
        std::string requests;
        std::string typeRequest;
        int error;
        void parse_request();
        void check_request(int client);
    public:
        request(/* args */);
        std::string getTypeRequest()
        {
            return typeRequest;
        }
        void receiveRequest(int client)
        {
            char buffer[3000];
            ssize_t bytesRead = read(client, buffer, sizeof(buffer) - 1);
            if (bytesRead == -1) {
                std::cerr << "socket read failed" << client;
                perror("read"); 
                close(client);
                return;
            }
            if (bytesRead == 0)
            {
                close(client);
                return;
            }
            buffer[bytesRead] = '\0';
            len = bytesRead;
            std::cout  << buffer;
            requests = buffer;
            parse_request();
            check_request(client);
            std::cout << "|" << url << "|"<< " " << "|" << typeRequest << "|"<< std::endl << std::endl;
            std::cout << "________________________________________________________" << std::endl << std::endl;
            
                
        }
        std::string getRequest()
        {
            return requests;
        }
        ~request();
        std::string getUrl()
        {
            return url;
        }
};
request::request(/* args */)
{
}

request::~request()
{
}

/* code */
#endif //REQUEST_HPP
