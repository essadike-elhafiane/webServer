#include "request.hpp"

void request::parse_request()
{
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
        std::ifstream r("html/404.html");
        if (!r.is_open())
        {
            std::cerr << "Error open file" << std::endl;
            exit(1);
        }
        std::string response;
        while (!std::getline(r, line).fail())
            response = response + line + '\n';
            rOK = rOK + std::to_string(response.length()) + "\r\n\r\n" + response;
        if (send(client, rOK.c_str(), rOK.length(), 0) < 0)
        {
            std::cerr << "Failed to send response." << std::endl;
            close(client);
            return;
        }
        return;
    }
    if (len > 2048)
    {
        error = 414;
        std::string rOK = "HTTP/1.1 414 Not Found\r\nCache-Control: no-cache\r\nContent-Length: ";
        std::ifstream r("html/414.html");
        if (!r.is_open())
        {
            std::cerr << "Error open file"  << std::endl;
            exit(1);
        }
        std::string response;
        while (!std::getline(r, line).fail())
            response = response + line + '\n';
            rOK = rOK + std::to_string(response.length()) + "\r\n\r\n" + response;
        if (send(client, rOK.c_str(), rOK.length(), 0) < 0)
        {
            std::cerr << "Failed to send response." << std::endl;
            close(client);
            return;
        }
        return;
    }
}

request::request(/* args */)
{
}

request::~request()
{
}
 