#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include "../Client/Client.hpp"
#include <sstream>
#include <vector>
#include <cstring>
#include "../response/response.hpp"
#include <cstdio>
class response;
class request
{
    private:
        std::string url;
        std::string Header;
        ssize_t pos;
        // int error;
        void parse_request(Client& dataClient, char *buffer, ssize_t& bytRead);
        void check_Get_Request(Client &dataClient);
        // void check_Post_Request(int client, Client& dataClient);
        response rsp;
    public:
        request(/* args */);
        void delete_request(Client& dataClient);
        void download_file(char *buffer , int bytesRead,Client &dataClient);
        void read_request(Client& dataClient);
        void receiveRequest(Client& dataClient)
        {
            read_request(dataClient);
            if (dataClient.getTypeRequset() == "POST" && dataClient.getReadlen() < dataClient.getContentLength())
                return ;
            if (dataClient.getTypeRequset() == "GET")
                check_Get_Request(dataClient);
            if (dataClient.getTypeRequset() == "DELETE")
                delete_request(dataClient);
            // std::cout << "|" << url << "|"<< " " << "|" << "|" << " " << dataClient.getClientSocket() << " "<< client << std::endl << std::endl;
            std::cout << std::endl << "________________________________________________________" << std::endl << std::endl;     
        }
        // std::string getRequest()
        // {
        //     return requests;
        // }
        ~request();
        std::string getUrl()
        {
            return url;
        }
};
 

/* code */
#endif //REQUEST_HPP
