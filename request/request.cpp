#include "request.hpp"

void request::parse_request(int client)
{
    (void)client;
    char* cstr = new char[requests.length() + 1];
    std::strcpy(cstr, requests.c_str());
    char* token = std::strtok(cstr, " ");
    std::vector<std::string> tokens;
    while (token != nullptr) {
        tokens.push_back(token);
        token = std::strtok(nullptr, " ");
    }
    delete[] cstr;
    // for (std::vector<std::string>::iterator itr = tokens.begin(); itr != tokens.end(); itr++) {
    //     std::cout << *itr << std::endl;
    // }
    typeRequest = tokens[0];
    url = tokens[1];
};

void request::check_request(int client)
{
    std::string line;
    std::string u = "/Users/eelhafia/Desktop/webServer" + url;
    
    if (access(u.c_str(), R_OK) && url != "/html/html/app-coder.html")
    {
        error = 404;
        std::string rOK = "HTTP/1.1 404 Not Found\r\nContent-Length: ";
        rsp.sendResponse("/Users/eelhafia/Desktop/webServer/html/404.html", rOK, client);
        return;
    }
}

request::request(/* args */)
{
}

request::~request()
{
}
 