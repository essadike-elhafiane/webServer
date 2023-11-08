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

class CGISettler {
private:
    std::vector<std::pair<std::string, std::string> > env;
    void addEnv(const std::string& key, const std::string& value) {
        envp[key] =  value;
    }
    std::string allrequest;// : These members could represent the path to the CGI script and the name of the CGI file
    std::string file;
    std::string output;
    std::string scriptType;
    std::map<std::string, std::string> envp;
    std::vector<char*> envpp;
    std::string cgi_exe;
    int R_pipes[2];
    int W_pipes[2];

public:

    Client &dataClient;
    std::string body;
    pid_t pid;
    CGISettler(std::string exe, const std::string& scriptType,  Client &dataClient);
    
    int getReadEnd() const;
    int getWriteEnd() const;
    // void processResponse();
    char* const*getEnv() const ;


    void executionCGI();
    void CgiEnv(Client &dataClient);
    bool validpath() const;
    // char** createEnvArray();
    void error_CGI();
    void close_pipes();
    // std::string stringQuery(std::string &restQuery);
    ~CGISettler();

};

std::string mainCGI(std::string cgi_exe, Client &dataClient);

#endif
