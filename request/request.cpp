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
        // std::cout << std::endl << "||"<< buffer <<"||" <<std::endl;
        delete[] cstr;
        bytRead = dataClient.getReadlen();
        // std::cout << "||"<< Header.size() << "||"  << poss + 4 << "||" << p << "||" << dataClient.getReadlen() << "||" << bytRead  << "||" <<std::endl;
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
    size_t pos_b = Header.find("boundary=", 0);
    if (pos_b != Header.npos)
    {
        size_t pos_bb = Header.find("\r\n", pos_b + 2);
        if (pos_bb != Header.npos)
            dataClient.setBoundaryRequest(Header.substr(pos_b + 9, pos_bb - pos_b - 9));
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

int findchar(const char *buffer, const char *dest, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        std::string s = buffer + i;
        size_t pos = s.find(dest, 0);
        if (pos != s.npos)
            return i + pos;
    }
    return -1;
}

void request::download_file(char *buffer , int bytesRead,Client &dataClient)
{
    std::size_t startPos = 0;

    if(dataClient.getFileName() == "" && dataClient.getHeaderStatus())
    {
        std::string request = buffer;

        startPos = request.find("\r\n\r\n", pos) + 4;
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
    
    }
    if (dataClient.getFileName() != "")
    {
        // std::cout << "dfghhhh d" << std::endl;
        std::string bonadry;
        bonadry = "--" + dataClient.getBoundarytSocket();
        std::size_t endPos = findchar(buffer + startPos + 1, bonadry.c_str(), bytesRead - startPos);
        if (endPos < 0 || endPos == bonadry.npos)
            endPos = 0;
        std::ofstream file(dataClient.getFileName(), std::ios::app | std::ios::binary);
        if (endPos == 0)
        {
            file.write(buffer + startPos, bytesRead - startPos);
            file.close();
            // std::cout << buffer +  endPos << std::endl;
        }
        else
        {
            // std::cout << buffer << std::endl;
            file.write(buffer + startPos, endPos - 1);
            file.close();
            std::string restR = buffer + endPos;
            bonadry = bonadry + "--";
            size_t pos = restR.find(bonadry);
            if (restR.find("\r\n\r\n") != restR.npos && pos == restR.npos)
            { 
                // std::cout << "gg" << std::endl;
                dataClient.setFileName("");
                download_file(buffer + endPos, bytesRead - endPos, dataClient);
            }
            else
            {
                size_t ppos = restR.find("\r\n\r\n");
                size_t pom = restR.find("\r\n\r\n", ppos + 4);
                if (ppos != restR.npos && pom != restR.npos)
                {
                    dataClient.setFileName("");
                    download_file(buffer + endPos, bytesRead - endPos, dataClient);
                }
            }
        }
    }
    
}

void printLoadingBar(int percentage, int barWidth) {
    std::cout << "Loading: [";
    
    int progress = (percentage * barWidth) / 100;
    
    for (int i = 0; i < barWidth; ++i) {
        if (i < progress) {
            std::cout << "=";
        } else if (i == progress) {
            std::cout << ">";
        } else {
            std::cout << " ";
        }
    }
    
    std::cout << "] " << percentage << "%";
    std::cout.flush();
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
        // std::cout << "|\r" << (double)(dataClient.getReadlen()) / dataClient.getContentLength() * 100 << "%" << std::endl;
        
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
                printLoadingBar((double)(dataClient.getReadlen()) / dataClient.getContentLength() * 100, 40);
                std::cout << "\r";
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
        }
        if (Header.find("\r\n\r\n") != std::string::npos && dataClient.getTypeRequset() == "GET")
            break;
    }

    
    if (dataClient.getReadlen() && dataClient.getContentLength() && dataClient.getReadlen() >= dataClient.getContentLength())
    {
        
        close(dataClient.getClientSocket());
        write(dataClient.getClientSocket(), "Content-Lenght 4\r\n\rdone" , 23);
        dataClient.resetData();
    }
    
}

void request::delete_request(Client& dataClient)
{
    std::string filename;
    filename = "download" + dataClient.getUrl();
    int result = std::remove(filename.c_str());
    if (result == 0) {
        std::string response1 = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
        rsp.sendResponse("/Users/eelhafia/Desktop/webServer/html/delete.html", response1, dataClient.getClientSocket());
        printf("File deleted successfully.");
        dataClient.resetData();
    } else {
        std::string response1 = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
        rsp.sendResponse("/Users/eelhafia/Desktop/webServer/html/not_delete.html", response1, dataClient.getClientSocket());
        printf("Failed to delete the file.\n");
        dataClient.resetData();
    }
}
