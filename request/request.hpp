#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include "../socketClient/socketClient.hpp"
#include <sstream>
#include <vector>
#include <cstring>

class request
{
    private:
        std::string url;
        int len;
        std::string requests;
        // char *buffe;
        std::string typeRequest;
        // int error;
    public:
        request(/* args */);
        std::string getTypeRequest()
        {
            return typeRequest;
        }
        void receiveRequest(socketClient client)
        {
            std::string line;
            
            char buffer[3000];
            ssize_t bytesRead = read(client.getClientSocket(), buffer, sizeof(buffer) - 1);
            if (bytesRead == -1) {
                std::cerr << "read failed" << std::endl;
                return;
            }
            buffer[bytesRead] = '\0';
            len = bytesRead;
            std::cout << "fg " << client.getClientSocket() << " " << buffer << std::endl;
            // len = recv(client.getClientSocket(), buffer, 30000 - 1, 0);
            // if (len == 0 || errno == EAGAIN || errno == EWOULDBLOCK)
            // {
            //     close(client.getClientSocket());
            // } 
            // if (len < 0)
            // {
            //     std::cerr << "Error recv: " << strerror(errno) << std::endl;
            //     close(client.getClientSocket());

            //     // exit(1);
            // }
            // buffer[len] = '\n';
            // buffer[len +1] = '\0';
            // requests = *buffer;
            // std::cout << buffer << std::endl;
            // char* cstr = new char[requests.length() + 1];
            // std::strcpy(cstr, requests.c_str());
            // char* token = std::strtok(cstr, " ");
            // std::vector<std::string> tokens;
            // while (token != nullptr) {
            //     tokens.push_back(token);
            //     token = std::strtok(nullptr, " ");
            // }

            // delete[] cstr;
            // for (std::vector<std::string>::iterator itr = tokens.begin(); itr != tokens.end(); itr++) {
            //     std::cout << *itr << std::endl;
            // }
            // typeRequest = tokens[0];
            // url = tokens[1];
            // std::cout << "|" << url << "|"<< " " << "|" << typeRequest << "|"<< std::endl;
            // std::string u = "/Users/eelhafia/Desktop/webServer" + url;
            // if (access(u.c_str(), R_OK))
            // {
            //     error = 404;
            //     std::string rOK = "HTTP/1.1 404 Not Found\r\nContent-Length: ";
            //     std::ifstream r("html/404.html");
            //     if (!r.is_open())
            //     {
            //         std::cerr << "Error open file" << std::endl;
            //         exit(1);
            //     }
            //     std::string response;
            //     std::string line; 
            //     while (!std::getline(r, line).fail())
            //         response = response + line + '\n';
            //      rOK = rOK + std::to_string(response.length()) + "\r\n\r\n" + response;
            //     // std::cout << std::endl << std::endl;
            //     // std::cout << rOK << std::endl;
            //     // std::cout << std::endl << "|" << requests.substr(4, 17) << "|" << std::endl << std::endl;
            //     if (send(client.getClientSocket(), rOK.c_str(), rOK.length(), 0) < 0)
            //     {
            //         std::cerr << "Failed to send response." << std::endl;
            //         close(client.getClientSocket());
            //         return;
            //     }
            //     close(client.getClientSocket());
            //         return;
            // }
            // if (len > 2048)
            // {
            //     error = 414;
                std::string rOK = "HTTP/1.1 414 Not Found\r\nContent-Length: ";
                std::ifstream r("html/414.html");
                if (!r.is_open())
                {
                    std::cerr << "Error open file" << std::endl;
                    exit(1);
                }
                std::string response;
                // std::string line; 
                while (!std::getline(r, line).fail())
                    response = response + line + '\n';
                 rOK = rOK + std::to_string(response.length()) + "\r\n\r\n" + response;
                std::cout << std::endl << std::endl;
                std::cout << rOK << std::endl;
            //     std::cout << std::endl << "|" << requests.substr(4, 17) << "|" << std::endl << std::endl;
                if (send(client.getClientSocket(), rOK.c_str(), rOK.length(), 0) < 0)
                {
                    std::cerr << "Failed to send response." << std::endl;
                    close(client.getClientSocket());
                    return;
                }
                close(client.getClientSocket());
            //     return;
            // }
                
        }
        std::string getRequest()
        {
            return requests;
        }
        ~request();
    std::string getUrl()
    {
        return url;
    }
};
request::request(/* args */)
{
}

request::~request()
{
}

/* code */
#endif //REQUEST_HPP
