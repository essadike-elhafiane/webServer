#include "CGI.hpp"
#include <sys/fcntl.h>
#include "../Client/Client.hpp"

CGISettler::CGISettler(std::string exe, const std::string& scriptType,Client &dataClient)
    :scriptType(scriptType) ,dataClient(dataClient)  {
    this->R_pipes[0] = -1;
    this->R_pipes[1] = -1;
    this->W_pipes[0] = -1;
    this->W_pipes[1] = -1;
    cgi_exe = exe;
    this->CgiEnv(dataClient);  
}





   void CGISettler::executionCGI () 
   {
        if (pipe(this->R_pipes) == -1 || pipe(this->W_pipes) == -1) {
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
        fcntl(getReadEnd(), F_SETFL, O_NONBLOCK, FD_CLOEXEC);
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
            // for (size_t i = 0; env[i]; i++)
            //     std::cerr << env[i] << std::endl;
            
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
 


 void CGISettler::CgiEnv(Client& dataClient) {
    
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
        if (!this->validpath())
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



bool CGISettler::validpath() const {
    std::ifstream in(file);
    if (!in.is_open())
        return false;
    return true;
}



int CGISettler::getReadEnd() const {
    return this->R_pipes[0];
}

int CGISettler::getWriteEnd() const {
    return this->W_pipes[1];
}

void CGISettler::error_CGI() {
    std::cerr << "Error Cgi\n";
    this->close_pipes();
    {
        dataClient.error = 500;
        throw "HTTP 500";
    }
}

void CGISettler::close_pipes() {
    if (this->R_pipes[0] != -1) {
        close(this->R_pipes[0]);
    }
    if (this->R_pipes[1] != -1) {
        close(this->R_pipes[1]);
    } 
    if (this->W_pipes[0] != -1) {
        close(this->W_pipes[0]);
    }
    if (this->W_pipes[1] != -1) {
        close(this->W_pipes[1]);
    }
}


char* const* CGISettler::getEnv() const {
    std::vector<char*> envp(env.size() + 1);
    for (size_t i = 0; i < env.size(); i++) {
        envp[i] = strdup((env[i].first + "=" + env[i].second).c_str());
    }
    envp[env.size()] = nullptr;
    char** envp_heap = new char*[envp.size()];
    std::copy(envp.begin(), envp.end(), envp_heap);

    return envp_heap;
    
}

CGISettler::~CGISettler() {}
