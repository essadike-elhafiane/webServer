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

class Client
{
    private:
        int clientSocket;
        std::string typeRequest;
        ssize_t readylen;
        ssize_t  Content_Length;
        std::string restRequest;
        std::string boundary;
        bool header;
    public:
        Client(/* args */);
        std::string getTypeRequset()
        {
            return typeRequest;
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
        void setRestRequest(std::string const& s)
        {
            restRequest = s;
        }
        void appendRestRequest(std::string const& s, ssize_t len)
        {
            (void)len;
            // if (len > 10000)
            //     exit(1);
            restRequest += s;
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
            clientSocket = 0;
            Content_Length = 0;
            restRequest = "";
            boundary = "";
            typeRequest = "";
        }
        ~Client();
};



#endif //SOCKET_HPP
