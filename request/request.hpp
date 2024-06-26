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

    public:
        request(/* args */);
        int parseChunck(Client& dataClient, size_t pos);
        void delete_request(Client& dataClient);
        int download_file(Client &dataClient, ssize_t pos_start);
        void read_request(Client& dataClient);
        int   checkAllowedMethods(Client& dataClient)
        {
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
                dataClient.setUrl(dataClient.configData.pages[i].root + dataClient.configData.pages[i].path + dataClient.getUrl());
                dataClient.path = dataClient.configData.pages[i].path;
                return 0;
            }
            else
            {
                    size_t i;
                    for (i = 0; i < dataClient.configData.pages.size(); i++)
                    {
                        std::string nameLocation = dataClient.getUrl().substr(0, dataClient.configData.pages[i].path.size());
                        if (dataClient.configData.pages[i].path == "/")
                            continue;
                        if (dataClient.configData.pages[i].path == nameLocation)
                            break;
                    }
                    if (i == dataClient.configData.pages.size())
                    {
                        size_t i;
                        for (i = 0; i < dataClient.configData.pages.size(); i++)
                            if (dataClient.configData.pages[i].path == "/")
                                break;
                        dataClient.setUrl(dataClient.configData.pages[i].root + dataClient.getUrl());
                        dataClient.path = dataClient.configData.pages[i].path;
                        if (std::find(dataClient.configData.pages[i].allow_methods.begin(), dataClient.configData.pages[i].allow_methods.end(), dataClient.getTypeRequset()) == dataClient.configData.pages[i].allow_methods.end())
                            return (dataClient.error = 403, 1);
                        return 0;
                    }
                    if (dataClient.configData.pages[i].path == dataClient.getUrl())
                    {
                        std::ifstream f(dataClient.configData.pages[i].root + dataClient.getUrl(), std::ios::out);
                        if (!f.is_open() && dataClient.getTypeRequset() != "POST" && dataClient.getTypeRequset() != "DELETE" )
                        {
                            dataClient.configData.pages[i].isredirection = 1;
                            dataClient.path = dataClient.getUrl();
                            dataClient.setUrl(dataClient.getUrl() + "/");
                            return 0;
                        }
                    }
                    if (std::find(dataClient.configData.pages[i].allow_methods.begin(), dataClient.configData.pages[i].allow_methods.end(), dataClient.getTypeRequset()) == dataClient.configData.pages[i].allow_methods.end())
                        return (dataClient.error = 403, 1);
                    dataClient.setUrl(dataClient.configData.pages[i].root + dataClient.getUrl());
                    dataClient.path = dataClient.configData.pages[i].path;
                    return 0;
                    
            }
            return 0;
        }

        void receiveRequest(Client& dataClient)
        {
            read_request(dataClient);
            // std::ofstream m("txt",std::ios::app);
            // m << dataClient.getRestRequest();
            if (dataClient.error)
                return ;
            if (dataClient.getTypeRequset() == "POST" && dataClient.getReadlen() < dataClient.getContentLength())
                return ;

            if (dataClient.getUrl().find(".py") != std::string::npos || dataClient.getUrl().find(".php") != std::string::npos) 
            {
                if (dataClient.getTypeRequset() != "POST" && dataClient.getTypeRequset() != "GET")
                {
                    dataClient.error = 403;
                    return ;
                }
                size_t i;
                for (i = 0; i < dataClient.configData.pages.size(); i++)
                    if (dataClient.configData.pages[i].path == dataClient.path)
                        break;
                if (i == dataClient.configData.pages.size())
                {
                    dataClient.error = 404;
                    return ;
                }
                std::string exe = dataClient.configData.pages[i].root + dataClient.configData.pages[i].path + dataClient.configData.pages[i].cgi_exe;
                std::string res =  mainCGI(exe, dataClient);
                dataClient.SetCgi(res);
                return ;
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
            if (dataClient.getTypeRequset() == "POST")
            {
                if (download_file(dataClient, 0))
                    return ;
                int s = dataClient.getClientSocket();
                dataClient.resetData();
                dataClient.error = 201;
                dataClient.path = "/";
                dataClient.setClientSocket(s);
            }
            if (dataClient.getTypeRequset() == "DELETE")
                delete_request(dataClient);
        
        }
        ~request();
};
 


#endif 
