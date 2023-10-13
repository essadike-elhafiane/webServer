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
        void delete_request(Client& dataClient)
        {
            std::string filename;
            filename = "download" + dataClient.getUrl();
            int result = std::remove(filename.c_str());
            if (result == 0) {
                std::string response1 = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
                rsp.sendResponse("/Users/eelhafia/Desktop/webServer/html/delete.html", response1, dataClient.getClientSocket());
                printf("File deleted successfully.");
            } else {
                std::string response1 = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
                rsp.sendResponse("/Users/eelhafia/Desktop/webServer/html/not_delete.html", response1, dataClient.getClientSocket());
                printf("Failed to delete the file.\n");
            }
        }
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
