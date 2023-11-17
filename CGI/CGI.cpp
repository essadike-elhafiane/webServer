#include "CGI.hpp"
#include <sys/fcntl.h>
#include "../Client/Client.hpp"

Web_Secript_Setter::Web_Secript_Setter(std::string exe, const std::string& scriptType,Client &dataClient)
    :scriptType(scriptType) ,dataClient(dataClient)  {
    this->R_pipes[0] = -1;
    this->R_pipes[1] = -1;
    cgi_exe = exe;
    this->Http_request_Env(dataClient);  
}


   void Web_Secript_Setter::executionCGI () 
   {
        if (pipe(this->R_pipes) == -1) {
            this->close_pipes();
            dataClient.error = 500;
            throw "HTTP 500";
        }

        std::ofstream m("/tmp/file111.tmp");
        int fd = open("/tmp/file111.tmp", O_CREAT | O_RDWR | O_TRUNC);
        if (fd < 0)
        {
            dataClient.error = 500;
            throw "HTTP 500";
        }
        
        if (write(fd, dataClient.getRestRequest().c_str() + posbody, dataClient.getRestRequest().size() - posbody) < 0)
        {
            dataClient.error = 500;
            throw "HTTP 500";
        }

        close(fd);
        fd = open("/tmp/file111.tmp", O_RDWR);
        pid_t pid = fork();
        if (pid == -1) {
            this->close_pipes();
            dataClient.error = 500;
            throw "HTTP 500 here";
        }
        if (fcntl(ReadValue(), F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
        {
            perror("fcnl failed");
            dataClient.error = 500;
            throw "HTTP 500 here";
        }
        this->pid = pid;
        if (pid == 0) {
           if (dup2(this->R_pipes[1], STDOUT_FILENO) == -1 ||
                dup2(fd, STDIN_FILENO) == -1)
                this->error_CGI();
            
            const char* bin;
            char* args[3];
    
            bin = cgi_exe.c_str(); 
            args[0] = (char*)bin;
            args[1] = (char*)this->file.c_str();
            args[2] = NULL;
            
            char **env = new char *[this->envp.size() + 1];
            env[this->envp.size()] = NULL;
            std::map<std::string, std::string>::const_iterator it = this->envp.begin();
            size_t i = 0;
            
            for (it = this->envp.begin(); it != this->envp.end(); it++) {
                env[i++] = strdup((it->first + "=" + it->second).c_str());
            }
            
            close_pipes();
            close(fd);
            if (execve(bin, args, env) == -1) {
                std::cerr << "Failed to execute the CGI script: " << strerror(errno) << std::endl;
                this->error_CGI();
                exit(1);
            }
           
            std::cerr << "Failed to execute the CGI script: " << strerror(errno) << std::endl;
            this->error_CGI();
            exit(1);
        }
        close(fd);
    }
 
 void Web_Secript_Setter::Http_request_Env(Client& dataClient) {
    
    size_t pos2 = dataClient.getRestRequest().find("Content-Type: ");
    std::string valueContentType =dataClient.getRestRequest().substr(pos2 + 14,dataClient.getRestRequest().find("\r\n", pos2) - pos2 - 14);

        size_t pos1 = dataClient.getUrl().find("?");
        std::string valuequertString;
        if (pos1 != std::string::npos) {
            valuequertString = dataClient.getUrl().substr(pos1 + 1);
        } else {
            valuequertString = ""; 
        }
        if (pos1 != std::string::npos)
            file = dataClient.getUrl().substr(0, pos1);
        else
            file = dataClient.getUrl();
        if (!this->Is_valid_path())
        {
            dataClient.error = 404;
            throw "HTTP  502";
        }
        size_t posCoockie = dataClient.getRestRequest().find("Cookie:");
        std::string valueCoockie;
        if (posCoockie != std::string::npos)
        {   
            valueCoockie = dataClient.getRestRequest().substr(posCoockie + 8, dataClient.getRestRequest().find("\r\n", posCoockie + 7) - posCoockie - 8);
        }
        env.clear();
      
        addEnv("CONTENT_TYPE", valueContentType); 
        if (!valuequertString.empty())
            addEnv("HTTP_QUERY_STRING",  valuequertString);
        addEnv("REQUEST_METHOD", dataClient.getTypeRequset()); 
        addEnv("SCRIPT_FILENAME", file);
        std::stringstream m;
        m << dataClient.getContentLength();
        addEnv("CONTENT_LENGTH", m.str());
        addEnv("REDIRECT_STATUS","200");
        if (posCoockie != std::string::npos)
            addEnv("HTTP_COOKIE", valueCoockie);
        
}



bool Web_Secript_Setter::Is_valid_path() const {
    std::ifstream in(file);
    if (!in.is_open())
        return false;
    return true;
}



int Web_Secript_Setter::ReadValue() const {
    return this->R_pipes[0];
}


void Web_Secript_Setter::error_CGI() {
    std::cerr << "Error Cgi\n";
    this->close_pipes();
    {
        dataClient.error = 500;
        throw "HTTP 500";
    }
}

void Web_Secript_Setter::close_pipes() {
    if (this->R_pipes[0] != -1) {
        close(this->R_pipes[0]);
    }
    if (this->R_pipes[1] != -1) {
        close(this->R_pipes[1]);
    } 
}


char* const* Web_Secript_Setter::getEnv() const {
    std::vector<char*> envp(env.size() + 1);
    for (size_t i = 0; i < env.size(); i++) {
        envp[i] = strdup((env[i].first + "=" + env[i].second).c_str());
    }
    envp[env.size()] = NULL;
    char** envp_heap = new char*[envp.size()];
    std::copy(envp.begin(), envp.end(), envp_heap);

    return envp_heap;
    
}

Web_Secript_Setter::~Web_Secript_Setter() {}
