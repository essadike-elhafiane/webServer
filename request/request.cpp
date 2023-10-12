#include "request.hpp"

void request::parse_request(Client& dataClient, char *buffer, ssize_t& bytRead)
{
    // std::cout << Header << "|" << std::endl;
    // (void)buffer;
    // (void)bytRead;
    if (Header == "")
        return ;
    if (Header.find("\r\n\r\n") != std::string::npos)
    {
        size_t poss = Header.find("\r\n\r\n");
        long int p = bytRead - poss - 4;
        if (p < 0)
            p = 0;
        dataClient.setReadlen(p);
        char* cstr = new char[bytRead + 1];
        memset(cstr, 0, bytRead);
        memcpy(cstr, buffer, bytRead);
        memset(buffer, 0, 3000);
        memcpy(buffer, cstr + poss + 4, dataClient.getReadlen());
        std::cout << std::endl << "||"<< buffer <<"||" <<std::endl;
        delete[] cstr;
        bytRead = dataClient.getReadlen();
        std::cout << "||"<< Header.size() << "||"  << poss + 4 << "||" << p << "||" << dataClient.getReadlen() << "||" << bytRead  << "||" <<std::endl;
    }
    char* cstr = new char[Header.length() + 1];
    std::strcpy(cstr, Header.c_str());
    char* token = std::strtok(cstr, " ");
    std::vector<std::string> tokens;
    while (token != nullptr) {
        tokens.push_back(token);
        token = std::strtok(nullptr, " \n");
    }
    delete[] cstr;
    std::istringstream requestStream(Header);
    std::string line;
    while (std::getline(requestStream, line)) {
        if (line.substr(0, 44) == "Content-Type: multipart/form-data; boundary=") {
            dataClient.setBoundaryRequest(line.substr(44, 39));
            // std::cout << "  |||||||| " <<std::endl;
            break;
        }
    }
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
    dataClient.setUrl(tokens[1]);
};

void request::check_Get_Request(Client &dataClient)
{
    std::string line;
    std::string u = "/Users/eelhafia/Desktop/webServer" + dataClient.getUrl();
    if (access(u.c_str(), R_OK) && dataClient.getUrl() != "/html/html/app-coder.html")
    {
        // error = 404;
        std::string rOK = "HTTP/1.1 404 Not Found\r\nContent-Length: ";
        rsp.sendResponse("/Users/eelhafia/Desktop/webServer/html/404.html", rOK, dataClient.getClientSocket());
        dataClient.setHeaderStatus(false);
        return;
    }
    // if (len > 2048 && typeRequest != "POST")
    // {
    //     error = 414;
    //     std::string rOK = "HTTP/1.1 414 Not Found\r\nCache-Control: no-cache\r\nContent-Length: ";
    //     rsp.sendResponse("/Users/eelhafia/Desktop/webServer/html/414.html", rOK, client);
    //     close(client);
    //     return;
    // }
    else 
    {
        if (dataClient.getUrl() == "/")
            dataClient.setUrl("/html/file.html");
        u = "/Users/eelhafia/Desktop/webServer" + dataClient.getUrl();
        std::cout << dataClient.getUrl() << std::endl;
        std::string rOK = "HTTP/1.1 200 OK\r\nContent-Length: ";
        rsp.sendResponse(u, rOK, dataClient.getClientSocket());
        dataClient.setHeaderStatus(false);
        // close(dataClient.getClientSocket());
        dataClient.setTypeRequset("");
        dataClient.setHeaderStatus(false);
        dataClient.setUrl("");
        return;
    }
}


request::request(/* args */)
{
    pos = 0;
    // len = 0;
}

request::~request()
{
}
 
void request::download_file(char *buffer , int bytesRead,Client &dataClient)
{
    if(dataClient.getFileName() == "" && dataClient.getHeaderStatus())
    {
        // std::cout << "||||" << buffer << "||" << std::endl;
        std::string request = buffer;
        std::size_t pos = request.find("filename=", 0);
        if (pos == request.npos)
            std::cout << "ddd |" << bytesRead << " " << buffer<< std::endl;
        std::size_t startPos = request.find("\r\n\r\n", pos) + 4;
        if (startPos == request.npos)
            std::cout << "AAA |" << bytesRead << std::endl;
        size_t p = request.find("filename=", 0) + 9;
        size_t po = request.find("\n", p);
        std::string namefile1 = request.substr(p, po - p + 1);
        std::string namefile = namefile1.substr(1, namefile1.size() - 4);
        namefile = "/Users/eelhafia/Desktop/webServer/download/" + namefile;
        dataClient.setFileName(namefile);
        std::ofstream file(namefile, std::ios::out | std::ios::binary);
        if (!file) {
            std::cerr << "Error opening file for writing" << namefile << std::endl;
            return;
        }
        file.write(buffer + startPos, bytesRead - startPos);
        file.close();
        std::cout << "dfg d" << std::endl;
    }
    else if (dataClient.getHeaderStatus())
    {
        std::string request = buffer;
        std::size_t endPos = request.find("--" + dataClient.getBoundarytSocket(), 0);
        if (endPos != request.npos && endPos >= 2)
            endPos -= 2;
        else
            endPos = 0;
        std::ofstream file(dataClient.getFileName(), std::ios::app | std::ios::binary);
        if (endPos == 0)
            file.write(buffer, bytesRead);
        else
        {
            file.write(buffer, endPos);
            // write(2, buffer, bytesRead);
            // write(2, "\n\n", bytesRead);
            // write(2, dataClient.getBoundarytSocket().c_str(), dataClient.getBoundarytSocket().size());
            // close(dataClient.getClientSocket());
            // dataClient.setClientSocket(0);
        }
    }
}

void    request::read_request(Client& dataClient)
{
    char buffer[3001];
    int len_read = 3000;
    std::memset(buffer, 0, sizeof(buffer));

    while (true) {
        ssize_t bytesRead = recv(dataClient.getClientSocket(), buffer, len_read, 0);
        if (bytesRead == 0) {
            std::cout << "connection closed by client " << dataClient.getClientSocket() << "\n";  
            close(dataClient.getClientSocket());   
            dataClient.resetData();             
            break;
        }
        if (bytesRead < 0)
            break;
        
        if (dataClient.getHeaderStatus() == true)
            dataClient.setReadlen(bytesRead);
        else
            Header = buffer;
        // std::cout << buffer << std::endl;
        if (dataClient.getHeaderStatus() == false && Header != "")
        {
            // std::cout << buffer << "||" << std::endl;
            parse_request(dataClient, buffer, bytesRead);
            std::cout << bytesRead << std::endl;
            dataClient.setHeaderStatus(true);
        }
        // std::ofstream outputFile;
        // std::string name ="test/f" + std::to_string(dataClient.getClientSocket()) + ".txt";
        // outputFile.open(name.c_str(), std::ios::app | std::ios::out); // Open file in append mode
        // if (outputFile.is_open() && dataClient.getTypeRequset() == "POST") {
        //     outputFile.write(buffer, bytesRead);
        //     outputFile.close();
        // }
        // exit(1);
    
        if (dataClient.getReadlen())
        {
            // std::cout << "|||||" << buffer << "|||||" << std::endl;
            if (dataClient.getTypeRequset() == "POST" && dataClient.getHeaderStatus() == true){
                
                    download_file(buffer , bytesRead, dataClient);
            }
        }

        std::memset(buffer, 0, sizeof(buffer));
        if (dataClient.getReadlen() && dataClient.getTypeRequset() == "POST" && dataClient.getReadlen() >= dataClient.getContentLength())
        {
            // close(dataClient.getClientSocket());
        
            std::string response1 = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 10 \r\n\r\n donne!!!!!";
            send(dataClient.getClientSocket(), response1.c_str(), response1.size(),0);
            close(dataClient.getClientSocket());   
            dataClient.resetData();
            // dataClient.resetData();
        }
        if (Header.find("\r\n\r\n") != std::string::npos && dataClient.getTypeRequset() == "GET")
            break;
    }

    // std::ofstream out("test.txt", std::ios::app | O_NONBLOCK); // Open file in append mode
    // if (out.is_open() && dataClient.getTypeRequset() == "POST") {
    //     out << dataClient.getClientSocket() << " | "<< dataClient.getContentLength() << " | " <<dataClient.getReadlen() <<  " | " << dataClient.getHeaderStatus() << std::endl;
    //     out.close();
    // }
    // else
    // {
    //     std::cout << "nt open file test " << std::endl;
    // }

    // std::cout << "sdgsg\n" << std::endl;
    if (dataClient.getReadlen() && dataClient.getContentLength() && dataClient.getReadlen() >= dataClient.getContentLength())
    {
        
        close(dataClient.getClientSocket());
        write(dataClient.getClientSocket(), "Content-Lenght 4\r\n\rdone" , 23);
        dataClient.resetData();
    }
    // std::cout << Header << " " << url << " " << typeRequest << " " << dataClient.getContentLength() << std::endl;
    // exit(1);
    // exit(1);
}

// void    request::check_Post_Request(int client, Client& dataClient)
// {
//     (void)client;
//     (void)dataClient;
//     std::string request = dataClient.getRestRequest();

//     // std::cout << request << std::endl;
    

//     std::size_t pos = request.find("filename=", 0);
//     std::size_t startPos = request.find("\r\n\r\n", pos) + 4;
//     std::size_t endPos = request.find("--" + dataClient.getBoundarytSocket(), startPos) - 2;
//     std::string fileData = request.substr(startPos, endPos - startPos + 1);
//     size_t p = request.find("filename=", 0) + 9;
//     size_t po = request.find("\n", p);
//     std::string namefile1 = request.substr(p, po - p + 1);
//     std::string namefile = namefile1.substr(1, namefile1.size() - 4);
//     namefile = "/Users/eelhafia/Desktop/webServer/download/" + namefile;
//     std::ofstream file(namefile, std::ios::out | std::ios::binary);
//     if (!file) {
//         std::cerr << "Error opening file for writing" << namefile << std::endl;
//         return;
//     }
//     file.write(fileData.c_str(), fileData.length() - 1);
//     file.close();

//     // Send the response
//     std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 6\r\n\r\n dooone";
//     send(client, response.c_str(), response.length(), 0);

//     // Close the connection
//     // close(client);
// }