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
        void read_header(Client& dataClient);
    
        // int read_request(Client& dataClient)
        // {
        //     // std::cout << client << std::endl;
        //     int buf = 10000;
        //     if (dataClient.getContentLength() < 10000)
        //         buf = dataClient.getContentLength();
        //     char buffer[buf];
        //     std::memset(buffer, 0, sizeof(buffer));
        //     while (true) {

        //         int bytesRead = recv(dataClient.getClientSocket(), buffer,  sizeof(buffer) - 1, 0);
        //         if (bytesRead <= 0)
        //             break;
        //         dataClient.appendRestRequest(buffer, bytesRead);
        //         dataClient.setReadlen(bytesRead);
        //         if (bytesRead >= dataClient.getContentLength())
        //             break;
        //     }
        //     // std::cout << "dfhdhdfhdfhdfhdf |||||||||| dfhd " <<std::endl;
        //     if (dataClient.getContentLength() > dataClient.getReadlen())
        //         return 1;
        //     std::cout  << dataClient.getReadlen() << "||||||||||" << dataClient.getTypeRequset() << "|||||||||" << dataClient.getContentLength() << "||||" << dataClient.getReadlen() << "|||||"  << std::cout;

        //     // exit(1);
        //     return 0;
        // }
        void receiveRequest(int client, Client& dataClient)
        {
            
            // dataClient.setRestRequest(requests.substr(pos, requests.size() - pos));
            // if(dataClient.getHeaderStatus() == false)
            // {
                read_header(dataClient);
                if (dataClient.getReadlen() < dataClient.getContentLength())
                    return ;
            // }
            // if (dataClient.getReadlen() < dataClient.getContentLength() && read_request(dataClient))
            //     return ;
            // std::cout << dataClient.getRestRequest() << "| dsgsdgs ||"  << dataClient.getTypeRequset() <<  std::endl;
            // exit(1);
            // typeRequest = "POST";
            if (dataClient.getTypeRequset() == "GET")
                check_Get_Request(client);
            if (dataClient.getTypeRequset() == "POST")
                check_Post_Request(client, dataClient);
            close(client);
            dataClient.resetData();
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
