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
        // int error;
        void parse_request(Client& dataClient);
        void check_Get_Request(Client &dataClient);
        // void check_Post_Request(int client, Client& dataClient);
        response rsp;
    public:
        request(/* args */);
        void delete_request(Client& dataClient);
        void download_file(Client &dataClient, ssize_t pos_start);
        void read_request(Client& dataClient);
        void receiveRequest(Client& dataClient)
        {
            read_request(dataClient);
            if (dataClient.error)
                return ;
            // std::cout << dataClient.getReadlen() << " | " << dataClient.getRestRequest().length() << std::endl;
            // return ;
            if (dataClient.getTypeRequset() == "POST" && dataClient.getReadlen() < dataClient.getContentLength())
                return ;
            // std::ofstream f("tttt.txt", std::ios::app);
            // f.write(dataClient.getRestRequest().c_str(), dataClient.getRestRequest().length());
            if (dataClient.getTypeRequset() == "POST" && dataClient.getReadlen() == dataClient.getContentLength())
            {
                download_file(dataClient, 0);
                int s = dataClient.getClientSocket();
                dataClient.resetData();
                dataClient.setUrl("/html/delete.html");
                dataClient.setClientSocket(s);
            }
            // if (dataClient.getTypeRequset() == "GET")
            //     check_Get_Request(dataClient);
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
        // std::string getUrl()
        // {
        //     return url;
        // }
};
 

/* code */
#endif //REQUEST_HPP
