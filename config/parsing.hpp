#ifndef PARSING_HPP
#define PARSING_HPP

#include <fstream>
#include <string>
#include<iostream>
#include <vector>
#include<cstring>
#include<map>
#include <sstream>

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
        std::string redirection;
        std::string cgi;
        LOCATION()
        {
            path = "";
            root = "";
            index = "";
            autoindex = -1;
            cgi = "";
            redirection = "";
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
        std::vector<int> port;
        std::vector<std::string> allow_methods;
        std::vector<LOCATION> pages;

    HTTP_SERVER()
    {
        server_name = "";
        root = "";
        client_max_body_size = -1;
        error_page_path = "";
    }
    // HTTP_SERVER & operator = (const HTTP_SERVER & other)
    // {
    //     if(this->server_name.empty())
    //         this->server_name = other.server_name;
    //     if(this->root.empty())
    //         this->root = other.root;
    // }
    

};



std::ostream& operator<<(std::ostream& output_stream, const  LOCATION  &location)
{
    output_stream <<   "location \n";
    output_stream << "path : "; 
    output_stream << location.path + "\n";
    output_stream << "index : ";  
    output_stream << location.index + "\n" ;
    output_stream << "autoindex : ";  
    output_stream << location.autoindex ;
    output_stream << " \nredirection : ";  
    output_stream << location.redirection + "\n" ;
    output_stream << "cgi : ";  
    output_stream << location.cgi + "\n" ;
    return output_stream;
}


std::ostream& operator<<(std::ostream& output_stream, const  HTTP_SERVER  &server)
{
    output_stream <<   "server_name " ;
    output_stream <<   server.server_name + "\n" ;
    output_stream <<   "root" ;
    output_stream <<   server.root + "\n" ;
    output_stream <<   "client_max_body_size " ;
    output_stream <<   server.client_max_body_size ;
    output_stream <<   "\n error_page_path " ;
    output_stream <<   server.error_page_path + "\n" ;
    output_stream <<   "port \n";

    for(std::vector<int>::const_iterator ptr =  server.port.begin() ; ptr != server.port.end() ; ptr++)
    {
        output_stream << *ptr ;
        output_stream << "\n";
    }
    output_stream <<   "error_page ";
    for(std::map<int , std::string >::const_iterator ptr = server.error_page.begin() ; ptr != server.error_page.end() ; ptr++)
    {
        output_stream << ptr->first ;
        output_stream <<  " "  + ptr->second + "\n";
    }
    output_stream <<   "allow_methods";
    for(std::vector<std::string>::const_iterator ptr = server.allow_methods.begin() ; ptr != server.allow_methods.end() ; ptr++)
        output_stream << *ptr + "\n";
    // output_stream <<   "location \n";
    for(std::vector<LOCATION>::const_iterator ptr = server.pages.begin() ; ptr != server.pages.end() ; ptr++)
        output_stream << *ptr ;
        output_stream << "\n";

    return output_stream;
}

std::vector<HTTP_SERVER>& configFile (int argc , char **argv);


#endif