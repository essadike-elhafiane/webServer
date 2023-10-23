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
#include "../CGI/CGI.hpp"
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
            std::cout << dataClient.getTypeRequset() << std::endl;
            // return ;
            if (dataClient.getTypeRequset() == "POST" && dataClient.getReadlen() < dataClient.getContentLength())
                return ;
            if(dataClient.getUrl().length() > 3)
            {
                if (dataClient.getUrl().find(".py", dataClient.getUrl().length() - 4) != dataClient.getUrl().npos) 
                {
                    // dataClient.cgi =  mainCGI(dataClient.getUrl(), dataClient.getClientSocket(),  dataClient);
                    int s = dataClient.getClientSocket();
                    dataClient.resetData();
                    dataClient.setClientSocket(s);
                    return ;
                }
                else if (dataClient.getUrl().find(".php", dataClient.getUrl().length() - 4) != dataClient.getUrl().npos)
                {
                    std::string res =  mainCGI(dataClient.getUrl(), dataClient.getClientSocket(), dataClient);
                    std::cout<<";;;;;;" << res;
                    dataClient.SetCgi(res);
                    // int s = dataClient.getClientSocket();
                    // dataClient.resetData();
                    // dataClient.setClientSocket(s);
                    std::cout << "php\n\n" << dataClient.getCgi() ;
                    return ;
                }
            }
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
        // std::string getUrl()
        // {
        //     return url;
        // }
};
 

/* code */
#endif //REQUEST_HPP
