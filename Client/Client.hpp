#ifndef SOCKETCLIENT_HPP
#define SOCKETCLIENT_HPP

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
#include "../server/server.hpp"
#include <set>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include "../config/parsing.hpp"

class Client
{
    private:
        int clientSocket;
        std::string typeRequest;
        ssize_t readylen;
        ssize_t  Content_Length;
        std::string filename;
        std::string restRequest;
        std::string boundary;
        std::string Url;
        bool header;
        // response variables
        std::streampos lenSend;
        std::string dataResponse;
        std::string cgi;
    public:
        HTTP_SERVER configData;
        std::vector<HTTP_SERVER> dataServers;
        std::string connection;
        clock_t timerequest;
        Client();
        void SetCgi(std::string  s)
        {
            for (size_t i = 0; i < s.size(); i++)
                cgi.push_back(s[i]);
            // //std::cout<< "||||||" << cgi << "!!!!!!!!!\n";
        }
        std::string &getCgi()
        {
            return cgi;
        }
        int error;
        std::streampos lengthFile; 
        std::string nameServer;
        int  port;
        std::string HostName;
        std::streampos getLenSend()
        {
            return lenSend;
        }
        void setLenSend(ssize_t len)
        {
            lenSend += len;
        }
        void clearLenSend()
        {
            lenSend = 0;
        }
        std::string getTypeRequset()
        {
            return typeRequest;
        }
        void setFileName(std::string name)
        {
            filename = name;
        }
        std::string getUrl()
        {
            return Url;
        }
        void setUrl(std::string url)
        {
            Url = url;
        }
        std::string getFileName()
        {
            return filename;
        }
        void setTypeRequset(std::string R)
        {
            typeRequest = R;
        }
        void setContentLength(ssize_t len)
        {
            Content_Length = len;
        }
        ssize_t getContentLength()
        {
            return Content_Length;
        }
        int getClientSocket()
        {
            return clientSocket;
        }
        std::string&  getRestRequest()
        {
            return restRequest;
        }
        void setRestRequest(char *buffer, ssize_t size)
        {
            for (ssize_t i = 0; i < size; i++)
                restRequest.push_back(buffer[i]);
        }
        void resetRestRequest()
        {
            restRequest.clear();    
        }
        std::string&  getdataResponse()
        {
            return dataResponse;
        }
        void setdataResponse(std::string s)
        {
           dataResponse = s;
        }
        void resetdataResponse()
        {
            dataResponse.clear();    
        }
        void setClientSocket(int socket)
        {
            clientSocket = socket;
        }
        void setBoundaryRequest(std::string const& s)
        {
            boundary = s;
        }
        std::string&  getBoundarytSocket()
        {
            return boundary;
        }
        bool getHeaderStatus()
        {
            return header;
        }
        void setReadlen(ssize_t len)
        {
            readylen += len;
        }
        ssize_t getReadlen()
        {
            return readylen;
        }
        void setHeaderStatus(bool h)
        {
            header = h;
        }
        void resetData()
        {
            header = false;
            readylen = 0;
            lenSend = 0;
            clientSocket = 0;
            Content_Length = 0;
            filename = "";
            restRequest = "";
            boundary = "";
            typeRequest = "";
            lenSend = 0;
            dataResponse = "";
            lengthFile = 0;
            error = 0;
            cgi = "";
            connection = "";
        }
        ~Client();
};



#endif //SOCKET_HPP
