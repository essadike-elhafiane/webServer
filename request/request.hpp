#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include "../Client/Client.hpp"
#include <sstream>
#include <vector>
#include <cstring>
#include "../response/response.hpp"

class response;
class request
{
    private:
        std::string url;
        std::string Header;
        ssize_t pos;
        // int error;
        void parse_request(Client& dataClient);
        void check_Get_Request(int client);
        void check_Post_Request(int client, Client& dataClient);
        response rsp;
    public:
        request(/* args */);
        void download_file(char *buffer , int bytesRead,Client &dataClient);
        void read_header(Client& dataClient);
        void receiveRequest(int client, Client& dataClient)
        {
            read_header(dataClient);
            if (dataClient.getReadlen() < dataClient.getContentLength())
                return ;
            if (dataClient.getTypeRequset() == "GET")
                check_Get_Request(client);
            // if (dataClient.getTypeRequset() == "POST")
            //     check_Post_Request(client, dataClient);
            // close(client);
            // dataClient.resetData();
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
