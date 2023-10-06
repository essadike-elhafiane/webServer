#ifndef PARSING_HPP
#define PARSING_HPP

#include <fstream>
#include <string>
#include<iostream>
#include <vector>
#include<cstring>

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


void pars(char *file);
#endif