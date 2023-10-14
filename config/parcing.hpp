#ifndef PARCING_HPP
#define PARCING_HPP

#include <fstream>
#include <string>
#include<iostream>
#include <vector>
#include<cstring>
#include<map>

#define     iter       std::vector<std::string>::iterator
#define     conf_data  std::map::<std::string , std::string>

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
        std::string path;
        std::string root ;
        std::string index ;
        int autoindex;
        std::string redection;
        std::string cgi;
        LOCATION()
        {
            path = "";
            root = "";
            index = "";
            autoindex = -1;
            cgi = "";
        }
};


class HTTP_SERVER
{
    public:
        std::string server_name;
        std::string root;
        long long int client_max_body_size ;
        std::string error_page_path;
        std::map<int , std::string> error_page;
        std::vector<int , int> port;
        std::vector<std::string> allow_methods;
        std::vector<LOCATION> pages;

    HTTP_SERVER()
    {
        std::string server_name = "";
        std::string root = "";
        long long int client_max_body_size = -1;
    }
    HTTP_SERVER & operator = (const HTTP_SERVER & other)
    {
        if(this->server_name.empty())
            this->server_name = other.server_name;
        if(this->root.empty())
            this->root = other.root;
    }

};






#endif