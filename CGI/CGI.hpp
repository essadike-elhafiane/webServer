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
struct HttpRequest {
    std::string request_data ;
};

// struct Clientt {
//     std::string response_data;
// };


class CGISettler {
private:
    HttpRequest& request;
     //Clientt& Client;

    std::vector<std::pair<std::string, std::string> > env;
    void addEnv(const std::string& key, const std::string& value) {
        env.push_back(std::make_pair(key, value));
    }

    std::string path;  // : These members could represent the path to the CGI script and the name of the CGI file
    std::string file;
    std::string output;
    std::string scriptType;
    std::map<std::string, std::string> envp;
    std::vector<char*> envpp;
    int R_pipes[2];
    int W_pipes[2];

public:
    Client &dataClient;
    CGISettler(const std::string& path, const std::string& file, const std::string& scriptType, HttpRequest& request, Client &dataClient);
    
    int getReadEnd() const;
    int getWriteEnd() const;
    // void processResponse();
    char* const*getEnv() const ;

   
    void executionCGI();
    void CgiEnv(Client &dataClient);
    bool validpath() const;
    char** createEnvArray();
    void error_CGI();
    void close_pipes();
    ~CGISettler();
   
};
//int mainCGI(std::string urlCgi, int clientSocket, client dataClient);

std::string mainCGI(std::string urlCgi, int clientSocket, Client &dataClient);

#endif // CGI_RESOLVER_HPP
