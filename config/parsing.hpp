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
    // void find(HTTP_SERVER configData ,std::vector<int , std::string>::iterator &it)
    // {
        
    // }
};

// std::ostream& operator<<(std::ostream& output_stream, const  LOCATION  &location)
// {
//     output_stream <<   "location \n";
//     output_stream << "path : "; 
//     output_stream << location.path + "\n";
//     output_stream << "index : ";  
//     output_stream << location.index + "\n" ;
//     output_stream << "autoindex : ";  
//     output_stream << location.autoindex ;
//     output_stream << " \nredirection : ";  
//     output_stream << location.redirection + "\n" ;
//     output_stream << "cgi : ";  
//     output_stream << location.cgi + "\n" ;
//     return output_stream;
// }


// std::ostream& operator<<(std::ostream& output_stream, const  HTTP_SERVER  &server)
// {
//     output_stream <<   "server_name " ;
//     output_stream <<   server.server_name + "\n" ;
//     output_stream <<   "root" ;
//     output_stream <<   server.root + "\n" ;
//     output_stream <<   "client_max_body_size " ;
//     output_stream <<   server.client_max_body_size ;
//     output_stream <<   "\n error_page_path " ;
//     output_stream <<   server.error_page_path + "\n" ;
//     output_stream <<   "port \n";

//     for(std::vector<int>::const_iterator ptr =  server.port.begin() ; ptr != server.port.end() ; ptr++)
//     {
//         output_stream << *ptr ;
//         output_stream << "\n";
//     }
//     output_stream <<   "error_page ";
//     for(std::map<int , std::string >::const_iterator ptr = server.error_page.begin() ; ptr != server.error_page.end() ; ptr++)
//     {
//         output_stream << ptr->first ;
//         output_stream <<  " "  + ptr->second + "\n";
//     }
//     output_stream <<   "allow_methods";
//     for(std::vector<std::string>::const_iterator ptr = server.allow_methods.begin() ; ptr != server.allow_methods.end() ; ptr++)
//         output_stream << *ptr + "\n";
//     // output_stream <<   "location \n";
//     for(std::vector<LOCATION>::const_iterator ptr = server.pages.begin() ; ptr != server.pages.end() ; ptr++)
//         output_stream << *ptr ;
//         output_stream << "\n";

//     return output_stream;
// }

std::vector<HTTP_SERVER>& configFile (int argc , char **argv,std::vector< HTTP_SERVER> &data);


#endif
