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

void request::check_Get_Request(int client)
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
    if (len > 2048 && typeRequest != "POST")
    {
        error = 414;
        std::string rOK = "HTTP/1.1 414 Not Found\r\nCache-Control: no-cache\r\nContent-Length: ";
        rsp.sendResponse("/Users/eelhafia/Desktop/webServer/html/414.html", rOK, client);
        close(client);
        return;
    }
    else 
    {
        if (url == "/")
            url = "/html/file.html";
        u = "/Users/eelhafia/Desktop/webServer" + url;
        std::string rOK = "HTTP/1.1 200 OK\r\nContent-Length: ";
        rsp.sendResponse(u, rOK, client);
        return;
        
    }
}

void    request::check_Post_Request(int client)
{
    // char buffer[3000];
    std::string request = requests;
    // std::memset(buffer, 0, sizeof(buffer) - 1);
    // while (true) {
    //     int bytesRead = recv(client, buffer, 3000 - 1, 0);
    //     if (bytesRead <= 0)
    //     break;
    //     request.append(buffer, bytesRead);
    //     if (request.find("\r\n\r\n") != std::string::npos)
    //     break;
    // }
    
    // Parse the request to extract the file data
    std::string boundary;
    std::istringstream requestStream(request);
    std::string line;
    while (std::getline(requestStream, line)) {
        if (line.substr(0, 19) == "Content-Type: multipart/form-data; boundary=") {
        boundary = line.substr(39);
        break;
        }
    }

    std::size_t pos = request.find("name=\"file\"") + 14;
    std::size_t startPos = request.find("\r\n\r\n", pos) + 4;
    std::size_t endPos = request.find("--" + boundary, startPos) - 2;
    std::string fileData = request.substr(startPos, endPos - startPos);

    // Save the file data to a file
    std::ofstream file("uploaded_file.txt", std::ios::out | std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file for writing" << std::endl;
        return;
    }
    file.write(fileData.c_str(), fileData.length());
    file.close();

    // Send the response
    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
    send(client, response.c_str(), response.length(), 0);

    // Close the connection
    // close(client);
}
request::request(/* args */)
{
}

request::~request()
{
}
 