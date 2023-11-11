#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <map>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include<vector>
#include <sys/stat.h>
#include "../response/response.hpp"
#include "../Client/Client.hpp"

class Web_Secript_Setter {
private:
    std::vector<std::pair<std::string, std::string> > env;
    void addEnv(const std::string& key, const std::string& value) {
        envp[key] =  value;
    }
    std::string allrequest;
    std::string file;
    std::string output;
    std::string scriptType;
    std::map<std::string, std::string> envp;
    std::vector<char*> envpp;
    std::string cgi_exe;
    int R_pipes[2];

public:

    Client &dataClient;
    std::string body;
    size_t posbody;
    pid_t pid;
    Web_Secript_Setter(std::string exe, const std::string& scriptType,  Client &dataClient);
    char* const*getEnv() const ;
    void executionCGI();
    void Http_request_Env(Client &dataClient);
    bool Is_valid_path() const;
    int ReadValue() const;
    int Writevalue() const;
    void error_CGI();
    void close_pipes();
    ~Web_Secript_Setter();

};

std::string mainCGI(std::string cgi_exe, Client &dataClient);

#endif
