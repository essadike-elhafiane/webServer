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
            size_t len = send(dataClient.getClientSocket(), dataClient.getCgi().c_str() , dataClient.getCgi().size(), 0);
            if (len < 0)
            {
                std::cerr << "Failed to send response." << std::endl;
                return ;
            }
            dataClient.SetCgi("");
        }

        void indexGenerator(std::string url, std::string &write)
        {
            std::string path = url;
            write = "<!DOCTYPE html>\n";
            write += "<html lang=\"en\">\n";
            write += "<head>\n";
            write += "<meta charset=\"UTF-8\">\n";
            write += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
            write += "<title>" + path + "</title>\n";
            write += "<h1>index " + path + "<h1>\n";
            write += "<hr>\n<ul>\n";
            DIR *directory = opendir(path.c_str());
            if(directory == NULL)
            {
                write += "<h1>error open path" + path + "<h1>\n";
                return ;
            }
                // delete directory->__dd_buf;
                // delete directory;
            struct dirent* file;
            while ((file = readdir(directory)) != NULL)
            {
                std::string d_name = file->d_name;
                write += "<li><div style=\" padding: 20px; color: rgb(224, 190, 141); margin: 5px;\"><a href=\"" + d_name + "\">" + d_name + "</a></div></li>\n" ; 
                // std::string name();
            }
            return ;
        }

        void headre(Client & dataClient , std::string &url , std::string & configResponse)
        {
            //std::cout<< dataClient.error << std::endl;
            std::map<int , std::string>::const_iterator it = dataClient.configData.error_page.begin();
            while(it != dataClient.configData.error_page.end())
            {
                if(it->first == dataClient.error)
                    break;
                it++;
            }

            if (dataClient.error == 0)
            {
                configResponse = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: ";
                return;
            }
            else if (dataClient.error == 400 && it != dataClient.configData.error_page.end())
            {
                url = dataClient.configData.error_page[400];
                configResponse = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nConnection: close\r\nContent-Length: ";
                return;
            }
            else if (dataClient.error == 401  && it != dataClient.configData.error_page.end())
            {
                url = dataClient.configData.error_page[401];
                configResponse = "HTTP/1.1 401 Unauthorized\r\nContent-Type: text/html\r\nConnection: close\r\nContent-Length: ";
                return;
            }
            else if (dataClient.error == 403 && it != dataClient.configData.error_page.end())
            {
                //std::cout<< dataClient.configData.error_page[403]<< "fhd\n" ;
                url = dataClient.configData.error_page[403];
                configResponse = "HTTP/1.1 403 Forbidden\r\nConnection: close\r\nContent-Length: ";
                return;
            }
            else if (dataClient.error == 404 && it != dataClient.configData.error_page.end())
            {
                url = dataClient.configData.error_page[404];
                configResponse = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: ";
                return;
            }
            else if (dataClient.error == 413 && it != dataClient.configData.error_page.end())
            {
                url = dataClient.configData.error_page[413];
                configResponse = "HTTP/1.1 502 Bad Gateway\r\nContent-Type: text/html\r\nConnection: close\r\nContent-Length: ";
                return;
            }
            else
            {
                url = dataClient.configData.error_page.begin()->second;
                configResponse = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nConnection: close\r\nContent-Length: ";
            }
            //std::cout<< url << std::endl;
        }

        int redirection (Client & dataClient , std::string & url)
        {

            std::string configResponse;
            std::vector<LOCATION>::iterator ptr = dataClient.configData.pages.begin();

            while(ptr != dataClient.configData.pages.end())
            {
                // std::cout << "---|" <<  ptr->root + ptr->path << "|" << "222" << std::endl;
                if(!ptr->redirection.empty() && !ptr->root.empty()  && url == ptr->root + "/")
                {
                    configResponse = "HTTP/1.1 302 Found\r\n";
                    std::string sit = ptr->redirection;
                    configResponse += "Location: " + sit + "\r\n";
                    dataClient.clearLenSend();
                    send(dataClient.getClientSocket(), configResponse.c_str(), configResponse.length(), 0);
                    dataClient.error = 1000;
                    return 1;
                }
                ptr++;
            }
            return 0;
        }

       int  outoindex(std::string &url  , Client & dataClient ,  std:: string &response)
        {
            if(url[url.length() - 1] !=  '/' )
                return 0;
            DIR *directory = opendir(url.c_str());
            // std::string root;
            if(directory == NULL)  
            {
                std::cout << "not found\n";
                return 0 ;
            }
            delete directory->__dd_buf;
            delete directory;     
            std::vector<LOCATION>::iterator ptr = dataClient.configData.pages.begin();
            while(ptr != dataClient.configData.pages.end())
            {
                std::cout<< "4---|" <<  url << "|" << ptr->root << "|" << ptr->autoindex << "|44---"<< std::endl;
                // if(ptr->path == "\"")
                //     root = ptr->root + "\"";
                if(ptr->redirection.empty() &&  url == ptr->root + "/" && ptr->autoindex == 0)
                {
                    std::cout<< "77---|\n";
                    dataClient.error = 401;
                    return 1;
                }
                if(ptr->redirection.empty() &&  url == ptr->root + "/" && !ptr->index.empty())
                 {
                    url +=  ptr->index;
                    return 0;
                }
                if( url == ptr->root + "/")
                    break;
                ptr++;
            }
            if (ptr == dataClient.configData.pages.end())
                return (0);
            std::cout << "fgh\n";
            indexGenerator(url  , response);   
            return 0;
        }

        int sendResponse(Client & dataClient)
        {
            std::string configResponse;
            std::string line;
            std::string url;
            std::string response;
            url = dataClient.getUrl();
            std::cout<< "0---|" <<  url << "|" << dataClient.getClientSocket() << "|" << dataClient.error << "|---"<< std::endl;
            if(!dataClient.error && redirection(dataClient , url))
                return 0;
            std::cout<< "1---|" <<  url << "|" << dataClient.getClientSocket() << "|" << dataClient.error << "|---"<< std::endl;
            if (!dataClient.error && outoindex(url , dataClient , response ))
            {
                // std::cout << dataClient.error << "||||||||||||||||||||" << std::endl;
                sendResponse(dataClient);
                return 0;
            }
            //  std::cout<< "2?---|" <<  url << "|" << dataClient.getClientSocket() << "|" << dataClient.error << "|---"<< std::endl;
            if(!response.empty())
            {
                //  std::cout<< "4---|" <<  url << "|" << dataClient.getClientSocket() << "|" << dataClient.error << "|---"<< std::endl;
                if (dataClient.error == 0)
                {
                    // std::cout<< "5---|" <<  url << "|" << dataClient.getClientSocket() << "|" << dataClient.error << "|---"<< std::endl;
                    //   std::cout<< "5---|" <<  configResponse << "|---"<< std::endl;
                    configResponse = "HTTP/1.1 200 OK\r\nContent-Length: " +  std::to_string(response.length())+ "\r\n\r\n" + response;
                    // std::cout<< "4---|" << configResponse  << "|---"<< std::endl;
                    send(dataClient.getClientSocket(), configResponse.c_str() , configResponse.length() , 0);
                    dataClient.error = 1000;
                    return 0 ;
                }
            }
            // std::cout<< "---|" <<  url << "|" << dataClient.getClientSocket() << "|" << dataClient.error << "|---"<< std::endl;
            if(!dataClient.getLenSend())
            {
                headre(dataClient , url , configResponse);
                std::ifstream r(url, std::ios::binary | std::ios::ate | std::ios::in | std::ios::out);
                if (!r.is_open())
                {
                    std::cout<< url << std::endl;
                    std::cerr << "Error open file1"  << std::endl;
                    dataClient.error = 404;
                    sendResponse(dataClient);
                    return 0;
                }
                configResponse = configResponse + std::to_string(r.tellg()) + "\r\n\r\n";
                dataClient.lengthFile = static_cast<size_t>(r.tellg());
                size_t len = send(dataClient.getClientSocket(), configResponse.c_str() , configResponse.length() , 0);
                if (len < 0)
                {
                    std::cerr << "Failed to send response." << std::endl;
                    return 0;
                }
                r.close();
            }   
            //std::cout<< dataClient.getLenSend() <<  "mm\n";   
            std::ifstream input(url, O_RDWR);
            char buffer[30001];
            if(!input.is_open())
            {
                std::cout<< "error on file" << url << std::endl;
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
                // //std::cout<< "fhd\n" << input.tellg() << "|" << dataClient.getLenSend()  << "|" << dataClient.lengthFile << std::endl;
                input.close();
                dataClient.clearLenSend();
                dataClient.error = 1000;
                return 0;
            }
            // std::cout<< dataClient.getClientSocket() << "|"<<static_cast<ssize_t>(input.tellg()) - dataClient.getLenSend() << "h\n";
            // std::cout<< "ttt\n";
            size_t len = send(dataClient.getClientSocket(), buffer , input.tellg() - dataClient.getLenSend(), 0);
            // std::cout<< "tttp\n";
            if (len < 0)
            {
                std::cerr << "Failed to send response." << std::endl;
                return 1;
                dataClient.error = 1000;
            }
            dataClient.setLenSend(len);
            input.close();
            if (dataClient.getLenSend() < dataClient.lengthFile)
                return 1;
            dataClient.clearLenSend();
            // dataClient.resetdataResponse();
            //std::cout<< "|" << url  << "|"<< std::endl;
            //std::cout<< "send success" << std::endl;
            return 0;
        }
        response(/* args */);
        ~response();
};
/* code */
#endif //RESPONSE_H
