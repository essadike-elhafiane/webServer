#include "CGI.hpp"
#include <sys/fcntl.h>

CGISettler::CGISettler(const std::string& CGI_path, const std::string& CGI_file, const std::string& scriptType,
                        HttpRequest& request, Client &dataClient)
    : request(request), path(CGI_path),file(CGI_file),scriptType(scriptType) ,dataClient(dataClient)  {
        
    this->R_pipes[0] = -1;
    this->R_pipes[1] = -1;
    this->W_pipes[0] = -1;
    this->W_pipes[1] = -1;
    
    if (!this->validpath())
        throw "HTTP  502";
    this->CgiEnv(dataClient);  
}





   void CGISettler::executionCGI () {
        if (pipe(this->R_pipes) == -1 || pipe(this->W_pipes) == -1) {
            this->close_pipes();
            throw "HTTP 500";
        }
        std::cout<< dataClient.getContentLength()<<std::endl;
            std::cout<< dataClient.getTypeRequset()<<std::endl;

        pid_t pid = fork();
        if (pid == -1) {
            this->close_pipes();
            throw "HTTP 500 here";
        }
       
        if (pid == 0) {
    
           if (dup2(this->R_pipes[1], STDOUT_FILENO) == -1 ||
                dup2(this->W_pipes[0], STDIN_FILENO) == -1)
                this->error_CGI();
            if (close(this->R_pipes[0]) == -1 ||
                close(this->R_pipes[1]) == -1 ||
                close(this->W_pipes[0]) == -1 ||
                close(this->W_pipes[1]) == -1)
                this->error_CGI();
            
            
            const char* bin;
            char* args[3];
            if (scriptType == "php") {
        
                bin = "/Users/eelhafia/Desktop/webServer/CGI/php-cgi"; 
                args[0] = (char*)bin;
                args[1] = (char*)this->file.c_str();
                args[2] = nullptr;
            } else if (scriptType == "python") {

                bin = "/Users/eelhafia/Desktop/webServer/CGI/py-cgi";
                args[0] = (char*)bin;
                args[1] = (char*)this->file.c_str();
                args[2] = nullptr;
            } else {
                std::cout << "Unsupported scriptType: "<< std::endl;
                this->error_CGI();
                //exit(1);// not use exit just return
            }

            char **env = new char *[this->envp.size() + 1];
            env[this->envp.size()] = nullptr;
            std::map<std::string, std::string>::const_iterator it = this->envp.begin();
            size_t i = 0;
            
            for (it = this->envp.begin(); it != this->envp.end(); it++) {
                env[i++] = (char *)strdup((it->first + "=" + it->second).c_str());
            }

        
            if (execve(bin, args, env) == -1) {
                std::cerr << "Failed to execute the CGI script: " << strerror(errno) << std::endl;
                this->error_CGI();
                //exit(1);
            }
           
            std::cerr << "Failed to execute the CGI script: " << strerror(errno) << std::endl;
            this->error_CGI();
            //exit(1);
        }

        if (close(this->R_pipes[1]) == -1 || close(this->W_pipes[0]) == -1)
            throw "HTTP 500";
}
 

void CGISettler::CgiEnv(Client& dataClient) {
    

        env.clear();
        addEnv("CONTENT_TYPE", "text/plain"); 
        addEnv("QUERY_STRING", this->request.request_data);
        addEnv("REQUEST_METHOD","Get"); 
        addEnv("SCRIPT_FILENAME", dataClient.getFileName());
        addEnv("CONTENT_LENGTH", std::to_string (dataClient.getContentLength()));
        addEnv("PATH_INFO", "/");
        addEnv("REDIRECT_STATUS","200");
        size_t pos = 0;
        while (pos < this->request.request_data.length()) {
            size_t endPos = this->request.request_data.find("\r\n", pos);
            if (endPos == std::string::npos)
                break;

            std::string headerLine = this->request.request_data.substr(pos, endPos - pos);
            size_t separatorPos = headerLine.find(":");
            if (separatorPos != std::string::npos) {
                std::string headerKey = "HTTP_" + headerLine.substr(0, separatorPos);
                std::replace(headerKey.begin(), headerKey.end(), '-', '_');
                std::string headerValue = headerLine.substr(separatorPos + 1);
                addEnv(headerKey, headerValue);
            }
            pos = endPos + 2;
        }
}

// bool CGISettler::validpath() const {
//     struct stat fileInfo;

//     if (stat(this->path.c_str(), &fileInfo) == 0) {
    
//         if (fileInfo.st_mode & S_IXUSR) {
//             return true; 
//         }
//     }
    
//     return false; 
//     std::cout<<"hhhhhhh"<<std::endl;
// }

bool CGISettler::validpath() const {
    struct stat fileInfo;

    std::cout << this->path << std::endl;

    if (stat(this->path.c_str(), &fileInfo) == 0) {
        if (fileInfo.st_mode & S_IXUSR) {
            return true;
        } else {
            // Handle the case where the file doesn't have execute permission.
            std::cout << "File does not have execute permission." << std::endl;
        }
    } else {
        // Handle the case where stat fails, e.g., the file doesn't exist.
        std::cout << "File not found or stat failed." << std::endl;
    }

    return false;
}








int CGISettler::getReadEnd() const {
    return this->R_pipes[0];
}

int CGISettler::getWriteEnd() const {
    return this->W_pipes[1];
}

void CGISettler::error_CGI() {
    this->close_pipes();
    //exit(1);
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
// void CGISettler::processResponse() {

//     Clientt.response_data += "Processed response from CGI";
// }


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
