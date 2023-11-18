#ifndef RESPONSE_H
#define RESPONSE_H

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h> 
#include <fcntl.h>
#include "../Client/Client.hpp"
class response
{
    private:
        /* data */
    public: 

        void sendStringResponse(Client &dataClient)
        {
            ssize_t len = send(dataClient.getClientSocket(), dataClient.getCgi().c_str() , dataClient.getCgi().size(), 0);
            if (len <= 0)
            {
                std::cerr << "Failed to send response." << std::endl;
                dataClient.error = 1000;
                return ;
            }
            dataClient.SetCgi("");
            dataClient.error = 1000;
        }

        void indexGenerator(std::string url, std::string &write, std::string pathLocation, Client& dataClient)
        {
            std::string path = url;
            write = "<!DOCTYPE html>\n";
            write += "<html lang=\"en\">\n";
            write += "<head>\n";
            write += "<meta charset=\"UTF-8\">\n";
            write += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
            write += "<title> AutoIndex </title>\n";
            write += "<h1>index " + path + "<h1>\n";
            write += "<hr>\n<ul>\n";
            DIR *directory = opendir(path.c_str());
            if(directory == NULL)
            {
                dataClient.error = 404;
                return ;
            }
            
            struct dirent* file;

            while ((file = readdir(directory)) != NULL)
            {
                std::string d_name = file->d_name;
                if (d_name == "." || d_name == "..")
                    continue;
                size_t pos = url.find(pathLocation);
                if (pos == std::string::npos)
                    pos = 0;
                std::string rest = url.substr(pos, url.size() - pos);
                if (pathLocation == "/")
                {
                    size_t i;
                    for (i = 0; i < dataClient.configData.pages.size(); i++)
                        if (dataClient.configData.pages[i].path == "/")
                            break;
                    if (i == dataClient.configData.pages.size())
                    {
                        dataClient.error = 404;
                        return ;
                    }
                    rest = url.substr(dataClient.configData.pages[i].root.size(), url.size() - dataClient.configData.pages[i].root.size());
                }
                if (rest[rest.size() -1] != '/')
                    rest += "/";
                if (rest[0] == '/' && rest[1] == '/')
                    rest = rest.substr(1,rest.size() - 1);
                write += "<li><div style=\" padding: 20px; color: rgb(224, 190, 141); margin: 5px;\"><a href=\"" + rest + d_name + "\">" + d_name + "</a></div></li>\n" ; 
            }
            delete directory->__dd_buf;
            close(directory->__dd_fd);
            delete directory;
            return ;
        }

        void  assigne_code(std::map<int , std::string> &s)
        {
            s[200] = "ok";
            s[201] = "Created";
            s[202] = "Accepted";
            s[204] = "No Content";
            s[307]= "Temporary Redirect";
            s[400] = "bad Request";
            s[401] = "Unauthorized";
            s[403] = "Forbidden";
            s[404] = "Not Found";
            s[408] = "Request Timeout";
            s[413] = "Content Too Large";
            s[500] = "Internal Server Error";
        }

        void headre(Client & dataClient , std::string &url , std::string & configResponse)
        {
            std::map<int , std::string> code;
            assigne_code(code);
            std::string key_error;
            std::string type_error;
            std::string key;
            std::string type;

            std::map<int , std::string>::const_iterator it = dataClient.configData.error_page.begin();
            while(it != dataClient.configData.error_page.end())
            {
                if(it->first == dataClient.error)
                    break;
                it++;
            }
            if(dataClient.error == 0)
            {
                type_error = code[200];
                size_t pos = url.find_last_of(".");
                if (std::string::npos != pos)
                {
                    key = url.substr(pos + 1 , url.length() - pos - 1);
                    type = dataClient.configData.Extensions[key];       
                }
                else
                    type = "text/plain";
                configResponse = "HTTP/1.1 200 " + code[200]  + "\r\nContent-Type: " + type  + "\r\nContent-Length: ";
                return ;
            }
            if(it != dataClient.configData.error_page.end())
            {
                std::stringstream ss;
                ss << dataClient.error;
                key_error = ss.str() ;
                url = dataClient.configData.error_page[dataClient.error];
                configResponse = "HTTP/1.1 " + key_error + " " + code[dataClient.error]  + "\r\nContent-Type: text/html\r\nConnection: close\r\nContent-Length: ";
                return;
            }
            else
            {
                url = dataClient.configData.error_page.begin()->second;
                std::stringstream ss;
                ss << dataClient.error;
                key_error = ss.str();
                configResponse = "HTTP/1.1 " + key_error + " " + code[dataClient.error]  + "\r\nContent-Type: text/html\r\nConnection: close\r\nContent-Length: ";
            }
        }
        
        int redirection (Client & dataClient)
        {
            std::string configResponse;
            std::vector<LOCATION>::iterator ptr = dataClient.configData.pages.begin();

            while(ptr != dataClient.configData.pages.end())
            {
                if (dataClient.path == ptr->path && ptr->isredirection && ptr->redirection.empty())
                {
                    configResponse = "HTTP/1.1 307 Temporary Redirect\r\n";
                    ptr->isredirection = 0;
                    configResponse += "Location: " + dataClient.getUrl() + "\r\n";
                    dataClient.clearLenSend();
                    ssize_t len = send(dataClient.getClientSocket(), configResponse.c_str(), configResponse.length(), 0);
                    dataClient.error = 1000;
                    if( len <= 0)
                    {
                        return 0;
                    }
                    return 1;
                }
                if(!ptr->redirection.empty() && dataClient.path == ptr->path)
                {
                    configResponse = "HTTP/1.1 307 Temporary Redirect\r\n";
                    std::string sit = ptr->redirection;
                    configResponse += "Location: " + sit + "\r\n";
                    dataClient.clearLenSend();
                    ssize_t len = send(dataClient.getClientSocket(), configResponse.c_str(), configResponse.length(), 0);
                    if( len <= 0)
                    {
                        return 0;
                    }
                    dataClient.error = 1000;
                    return 1;
                }
                ptr++;
            }
            return 0;
        }

       int  outoindex(std::string &url  , Client & dataClient ,  std:: string &response)
        {
            DIR *directory = opendir(url.c_str());
            if(directory == NULL)
                return 0 ;
            delete directory->__dd_buf;
            close (directory->__dd_fd);
            delete directory;     
            std::vector<LOCATION>::iterator ptr = dataClient.configData.pages.begin();
                           

            while(ptr != dataClient.configData.pages.end())
            {
                if(ptr->redirection.empty() &&  ptr->index.empty() && ptr->autoindex == 0 && dataClient.path == ptr->path)
                {
                    dataClient.error = 404;
                    return 1;
                }
                if(ptr->redirection.empty() && !ptr->index.empty() && dataClient.path == ptr->path)
                 {
                    url +=  ptr->index;
                    return 0;
                }
                if(dataClient.path == ptr->path && ptr->autoindex == 1)
                    break;
                ptr++;
            }
            if (ptr == dataClient.configData.pages.end())
                return (0);
            indexGenerator(url  , response, dataClient.path, dataClient);   
            return 0;
        }

        int sendResponse(Client & dataClient)
        {
        
            std::string configResponse;
            std::string url;
            std::string response;
            url = dataClient.getUrl();
            if(!dataClient.error && redirection(dataClient))
                return 0;
            if (!dataClient.error && outoindex(url , dataClient , response ))
            {
                sendResponse(dataClient);
                return 0;
            }
            if(!response.empty())
            {
                if (dataClient.error == 0)
                {
                    std::stringstream m;
                    m << response.length();
                    configResponse = "HTTP/1.1 200 OK\r\nContent-Length: " +  m.str() + "\r\n\r\n" + response;
                    ssize_t len = send(dataClient.getClientSocket(), configResponse.c_str() , configResponse.length() , 0);
                    if (len < 0)
                        {
                            std::cerr << "Failed to send response." << std::endl;
                            dataClient.error = 1000;
                            return 0;
                        }
                        if(len == 0)
                            return 0;
                    dataClient.error = 1000;
                    return 0 ;
                }
            }
            if(!dataClient.getLenSend())
            {
                headre(dataClient , url , configResponse);
                std::ifstream r(url, std::ios::binary | std::ios::ate | std::ios::in | std::ios::out);
                if (!r.is_open())
                {
                    if (!dataClient.error)
                    {
                        dataClient.error = 404;
                        sendResponse(dataClient);
                    }
                    else if (dataClient.error)
                    {
                        std::map<int , std::string> code;
                        assigne_code(code);
                        std::stringstream ss;
                        ss << dataClient.error;
                        configResponse = "HTTP/1.1 "+ ss.str() + " " + code[dataClient.error] + "\r\nContent-Type: text/html\r\nConnection: close\r\nContent-Length: 34\r\n\r\n<!DOCTYPE html>\n<h1>Error : " + ss.str() + "</h1>";
                        ssize_t len = send(dataClient.getClientSocket(), configResponse.c_str() , configResponse.length() , 0);
                        if (len < 0)
                        {
                            std::cerr << "Failed to send response." << std::endl;
                            dataClient.error = 1000;
                            return 0;
                        }
                        if(len == 0)
                            return 0;
                        return 0;
                    }
                    std::cerr << "Error open file"  << std::endl;
                    if (dataClient.error == 404)
                        return 0;
                    return 0;
                }
                std::stringstream m;
                m << r.tellg();
                configResponse = configResponse + m.str() + "\r\n\r\n";
                dataClient.lengthFile = static_cast<size_t>(r.tellg());
                ssize_t len = send(dataClient.getClientSocket(), configResponse.c_str() , configResponse.length() , 0);
                if (len < 0)
                {
                    std::cerr << "Failed to send response." << std::endl;
                    dataClient.error = 1000;
                    r.close();
                    return 0;
                }
                if (len == 0)
                {
                    dataClient.error = 1000;
                    r.close();
                    return 0;
                }
                r.close();
            }    
            std::ifstream input(url, O_RDWR);
            char buffer[30001];
            if(!input.is_open())
            {
                dataClient.error = 1000;
                return 0;
            }
            input.seekg(dataClient.getLenSend());
            size_t lenRead = 30000;
            if (dataClient.lengthFile - dataClient.getLenSend() < 30000)
                lenRead = dataClient.lengthFile - dataClient.getLenSend();
            input.read(buffer, lenRead);
            if ((input.tellg() <= 0))
            {
                input.close();
                dataClient.clearLenSend();
                dataClient.error = 1000;
                return 0;
            }
            ssize_t len= send(dataClient.getClientSocket(), buffer , input.tellg() - dataClient.getLenSend(), 0);
            if (len <= 0)
            {
                std::cerr << "Failed to send response." << std::endl;
                input.close();
                dataClient.error = 1000;
                return 1;
            }
            dataClient.setLenSend(len);
            input.close();
            if (dataClient.getLenSend() < dataClient.lengthFile)
                return 1;
            dataClient.clearLenSend();
            return 0;
        }
        response(/* args */);
        ~response();
};

#endif 

/*

*/