#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include "../socketClient/socketClient.hpp"
#include <sstream>
#include <vector>
#include <cstring>
#include "../response/response.hpp"

class response;
class request
{
    private:
        std::string url;
        ssize_t len;
        std::string requests;
        std::string typeRequest;
        int error;
        void parse_request();
        void check_Get_Request(int client);
        void check_Post_Request(int client);
        response rsp;
    public:
        request(/* args */);
        std::string getTypeRequest()
        {
            return typeRequest;
        }
        void receiveRequest(int client)
        {
            char buffer[30];
            std::memset(buffer, 0, sizeof(buffer) - 1);
            while (true) {
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
                requests.append(buffer, bytesRead);
                len = bytesRead;
                if (requests.find("\r\n\r\n") != std::string::npos)
                    break;
            }
            std::cout  << requests;
            parse_request();
            if (typeRequest == "GET")
                check_Get_Request(client);
            else if (typeRequest == "POST")
                check_Post_Request(client);
            // std::cout << "|" << url << "|"<< " " << "|" << typeRequest << "|"<< std::endl << std::endl;
            std::cout << std::endl << "________________________________________________________" << std::endl << std::endl;     
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
 

/* code */
#endif //REQUEST_HPP
