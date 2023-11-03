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
        void parse_request(Client& dataClient);
        // void check_Get_Request(Client &dataClient);

    public:
        request(/* args */);
        int parseChunck(Client& dataClient, size_t pos);
        void delete_request(Client& dataClient);
        int download_file(Client &dataClient, ssize_t pos_start);
        void read_request(Client& dataClient);
        int   checkAllowedMethods(Client& dataClient)
        {
            std::cout << dataClient.getUrl() << std::endl;
            if (dataClient.error)
                return 1;
            if (dataClient.getUrl() == "/")
            {
                size_t i;
                for (i = 0; i < dataClient.configData.pages.size(); i++)
                    if (dataClient.configData.pages[i].path == "/")
                        break;
                if (i == dataClient.configData.pages.size()) 
                    return (dataClient.error = 404, 1);
                if (std::find(dataClient.configData.pages[i].allow_methods.begin(), dataClient.configData.pages[i].allow_methods.end(), dataClient.getTypeRequset()) == dataClient.configData.pages[i].allow_methods.end())
                    return (dataClient.error = 403, 1);
                dataClient.setUrl(dataClient.configData.pages[i].root + dataClient.getUrl());
                dataClient.path = dataClient.configData.pages[i].path;
                return 0;
            }
            else
            {
                size_t pos = dataClient.getUrl().find("/", 1);
                if (pos == std::string::npos)
                    pos = dataClient.getUrl().size();
                {
                    std::string nameLocation = dataClient.getUrl().substr(0, pos);
                    size_t i;
                    for (i = 0; i < dataClient.configData.pages.size(); i++)
                        if (dataClient.configData.pages[i].path == nameLocation)
                            break;
                    if (i < dataClient.configData.pages.size())
                    {
                        if (std::find(dataClient.configData.pages[i].allow_methods.begin(), dataClient.configData.pages[i].allow_methods.end(), dataClient.getTypeRequset()) == dataClient.configData.pages[i].allow_methods.end())
                            return (dataClient.error = 403, 1);
                        dataClient.setUrl(dataClient.configData.pages[i].root + dataClient.getUrl().substr(pos, dataClient.getUrl().size() - pos));
                        dataClient.path = dataClient.configData.pages[i].path;
                        return 0;
                    }
                }
                size_t i;
                for (i = 0; i < dataClient.configData.pages.size(); i++)
                    if (dataClient.configData.pages[i].path == "/")
                        break;
                if (i == dataClient.configData.pages.size())
                    return (dataClient.error = 404, 1);
                if (std::find(dataClient.configData.pages[i].allow_methods.begin(), dataClient.configData.pages[i].allow_methods.end(), dataClient.getTypeRequset()) == dataClient.configData.pages[i].allow_methods.end())
                    return (dataClient.error = 403, 1);
                dataClient.setUrl(dataClient.configData.pages[i].root + dataClient.getUrl());
                dataClient.path = dataClient.configData.pages[i].path;
            }
            return 0;
        }

        void receiveRequest(Client& dataClient)
        {
            read_request(dataClient);
            if (dataClient.error)
                return ;

            if (dataClient.getTypeRequset() == "POST" && dataClient.getReadlen() < dataClient.getContentLength())
                return ;
            
            if(dataClient.getUrl().length() > 3)
            {
                if (dataClient.getUrl().find(".py", dataClient.getUrl().length() - 4) != dataClient.getUrl().npos) 
                {
                    // dataClient.cgi =  mainCGI(dataClient.getUrl(), dataClient.getClientSocket(),  dataClient);
                    std::string res =  mainCGI(dataClient.getUrl(), dataClient.getClientSocket(), dataClient);
                    dataClient.SetCgi(res);
                    return ;
                }
                else if (dataClient.getUrl().find(".php", dataClient.getUrl().length() - 4) != dataClient.getUrl().npos)
                {
                    std::string res =  mainCGI(dataClient.getUrl(), dataClient.getClientSocket(), dataClient);
                    dataClient.SetCgi(res);
                    //std::cout<< "fgf=++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
                    return ;
                }
            }
            if (dataClient.getHeaderStatus() == true && dataClient.TransferEncoding == "chunked" && dataClient.getTypeRequset() == "POST")
            {
                if (parseChunck(dataClient, 0))
                    return ;
            }
            if (dataClient.getContentLength() != dataClient.getReadlen())
            {
                dataClient.error = 400;
                return ;
            }
            std::ofstream m("txt");
            m << dataClient.getRestRequest();
            std::cout << "||||||" << dataClient.getReadlen() << "|" << dataClient.getContentLength() << "||||\n";
            if (dataClient.getTypeRequset() == "POST" && dataClient.getReadlen() == dataClient.getContentLength())
            {
                // std::cout << "dhfhdfhd\n";
                if (download_file(dataClient, 0))
                    return ;
                // std::cout << "downnnnnnnnn\n";
                int s = dataClient.getClientSocket();
                dataClient.resetData();
                size_t i;
                for (i = 0; i < dataClient.configData.pages.size(); i++)
                    if (dataClient.configData.pages[i].path == "/download")
                        break;
                
                dataClient.setUrl(dataClient.configData.pages[i].root + dataClient.configData.pages[i].index);
                dataClient.path = dataClient.configData.pages[i].path;
                dataClient.setClientSocket(s);
            }
            if (dataClient.getTypeRequset() == "DELETE")
                delete_request(dataClient);
            // //std::cout<< "|" << url << "|"<< " " << "|" << "|" << " " << dataClient.getClientSocket() << " "<< client << std::endl << std::endl;
            //std::cout<< std::endl << "________________________________________________________" << std::endl << std::endl;     
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
