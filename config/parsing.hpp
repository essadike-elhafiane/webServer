#ifndef PARSING_HPP
#define PARSING_HPP

#include <fstream>
#include <string>
#include<iostream>
#include <vector>
#include<cstring>
#include<map>
#include <sstream>
#include <dirent.h>
#include <unistd.h>


class Mycfg
{
    public:
        std::vector<std::string> line;
        std::vector<std::string> line1;
        std::vector<std::string> line2;
        std::vector<std::string> line3;
        std::vector<std::string> line4;
        int bracket;
        int ip[4];
        int port;
        int host[4];
        std::string server_name;
};

class LOCATION
{
    public:
        std::vector<std::string> allow_methods;
        std::string path;
        std::string root ;
        std::string index ;
        int autoindex;
        std::string redirection;
        std::string cgi;
        int isredirection;
        LOCATION()
        {
            path = "";
            root = "";
            index = "";
            autoindex = 0;
            isredirection = 0;
            cgi = "";
            redirection = "";
        }
        LOCATION & operator = (const LOCATION & other)
        {
            if (this != &other)
            {
                this->path = other.path;
                this->root = other.root;
                this->index = other.index;
                this->autoindex = other.autoindex;
                this->redirection = other.redirection;
                this->cgi = other.cgi;
                this->allow_methods = other.allow_methods;
            }
            return *this;
        }
};

class HTTP_SERVER
{
    public:
        std::map<std::string, std::string> Extensions;
        std::string server_name;
        std::string hostname;
        long long int client_max_body_size ;
        std::map<int , std::string> error_page;
        std::vector<int> port;
        std::vector<LOCATION> pages;
        HTTP_SERVER()
        {
            hostname = "0.0.0.0";
            server_name = "";
            client_max_body_size = -1;
        }

        HTTP_SERVER & operator = (const HTTP_SERVER & other)
        {
                if (this != &other)
                {
                    this->server_name = other.server_name;
                    
                    this->client_max_body_size = other.client_max_body_size;
                    this->error_page = other.error_page;
                    this->port = other.port;
                    this->pages = other.pages;   
                }
            return *this;
        }
};


std::vector<HTTP_SERVER>& configFile (int argc , char **argv,std::vector< HTTP_SERVER> &data);


#endif
