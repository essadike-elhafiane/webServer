#include "request.hpp"

void request::parse_request(Client& dataClient)
{
    char* cstr = new char[Header.length() + 1];
    std::strcpy(cstr, Header.c_str());
    char* token = std::strtok(cstr, " ");
    std::vector<std::string> tokens;
    while (token != nullptr) {
        tokens.push_back(token);
        token = std::strtok(nullptr, " \n");
    }
    delete[] cstr;
    // for (std::vector<std::string>::iterator itr = tokens.begin(); itr != tokens.end(); itr++) {
    //     std::cout << *itr << std::endl;
    // }
    size_t pos = Header.find("Content-Length:", 0) + 16;
    size_t poss = Header.find("\n", pos);
    std::string k = Header.substr(pos, poss - pos + 1);
    std::string kk = k.substr(0, k.size() -2 );
    // std::cout << "|" << kk << "|" << std::endl;
    dataClient.setContentLength(std::atoi(kk.c_str()));
    dataClient.setTypeRequset(tokens[0]);
    url = tokens[1];
};

void request::check_Get_Request(int client)
{
    std::string line;
    std::string u = "/Users/eelhafia/Desktop/webServer" + url;
    // if (access(u.c_str(), R_OK) && url != "/html/html/app-coder.html")
    // {
    //     error = 404;
    //     std::string rOK = "HTTP/1.1 404 Not Found\r\nContent-Length: ";
    //     rsp.sendResponse("/Users/eelhafia/Desktop/webServer/html/404.html", rOK, client);
    //     return;
    // }
    // if (len > 2048 && typeRequest != "POST")
    // {
    //     error = 414;
    //     std::string rOK = "HTTP/1.1 414 Not Found\r\nCache-Control: no-cache\r\nContent-Length: ";
    //     rsp.sendResponse("/Users/eelhafia/Desktop/webServer/html/414.html", rOK, client);
    //     close(client);
    //     return;
    // }
    // else 
    // {
        if (url == "/")
            url = "/html/file.html";
        u = "/Users/eelhafia/Desktop/webServer" + url;
        std::string rOK = "HTTP/1.1 200 OK\r\nContent-Length: ";
        rsp.sendResponse(u, rOK, client);
        return;
    // }
}

void    request::check_Post_Request(int client, Client& dataClient)
{
    (void)client;
    (void)dataClient;
    std::string request = dataClient.getRestRequest();
    // std::cout << request << std::endl;
    std::istringstream requestStream(request);
    std::string line;
    while (std::getline(requestStream, line)) {
        // std::cout << "||" << line.substr(0, 44) << "||" << std::endl;
        if (line.substr(0, 44) == "Content-Type: multipart/form-data; boundary=") {
            dataClient.setBoundaryRequest(line.substr(44, 39));
            // std::cout << "  |||||||| " <<std::endl;
            break;
        }
    }
    // exit(1);
    // std::cout << "|" << request << "|" << " " << request.length() <<" | " << std::endl;
    std::size_t pos = request.find("filename=", 0);
    std::size_t startPos = request.find("\r\n\r\n", pos) + 4;
    // if(startPos ==  request.npos)
    //     return;
    std::size_t endPos = request.find("--" + dataClient.getBoundarytSocket(), startPos) - 2;
    std::string fileData = request.substr(startPos, endPos - startPos);
    size_t p = request.find("filename=", 0) + 9;
    size_t po = request.find("\n", p);
    std::string namefile1 = request.substr(p, po - p + 1);
    std::string namefile = namefile1.substr(1, namefile1.size() - 4);
    namefile = "/Users/eelhafia/Desktop/webServer/download/" + namefile;
    std::ofstream file(namefile, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file for writing" << namefile << std::endl;
        return;
    }
    fcntl(file.in, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    std::cout << fileData <<  std::endl << fileData.length() << " | " << dataClient.getContentLength() << std::endl;;
    file << fileData;
    file.close();

    // Send the response
    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n dooone";
    send(client, response.c_str(), response.length(), 0);

    // Close the connection
    close(client);
}

request::request(/* args */)
{
    pos = 0;
    // len = 0;
}

request::~request()
{
}
 

void    request::read_header(Client& dataClient)
{
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));
    while (true) {
        ssize_t bytesRead = recv(dataClient.getClientSocket(), buffer, 1, 0);
        if (bytesRead <= 0) {
            break;
        }
        dataClient.appendRestRequest(buffer, bytesRead);
        dataClient.setReadlen(bytesRead);
        Header.append(buffer, bytesRead);
        // if (bytesRead >= dataClient.getContentLength())
        //     break;
        // if (Header.find("\r\n\r\n") != std::string::npos)
        // {
        //     pos = Header.find("\r\n\r\n");
        //     break;
        // }
    }
    if (dataClient.getHeaderStatus() == false)
        parse_request(dataClient);
    dataClient.setHeaderStatus(true);
    std::ofstream outputFile;
    outputFile.open("test.txt", std::ios::app); // Open file in append mode

    if (outputFile.is_open()) {
        outputFile << dataClient.getClientSocket() << " | "<< dataClient.getContentLength() << " | "<<dataClient.getReadlen() <<  " | " << dataClient.getHeaderStatus() << std::endl;
        outputFile.close();
    }
    // std::cout << Header << " " << url << " " << typeRequest << " " << dataClient.getContentLength() << std::endl;
    // exit(1);
    // exit(1);
}
