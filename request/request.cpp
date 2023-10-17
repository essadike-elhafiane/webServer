#include "request.hpp"

void request::parse_request(Client& dataClient)
{
    size_t poss = dataClient.getRestRequest().find("\r\n\r\n");
    long int p = dataClient.getRestRequest().length() - poss - 4;
    if (p < 0)
        p = 0;
    dataClient.setReadlen(p);
    std::stringstream ss(dataClient.getRestRequest().substr(0, poss));
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
        if (tokens.size() > 2)
            break;
    }
    // std::cout << "hhh |" << dataClient.getRestRequest() << "|" << std::endl;
    dataClient.setTypeRequset(tokens[0]);
    dataClient.setUrl(tokens[1]);
    if (tokens[0] == "POST")
    {
        size_t pos_boundary = dataClient.getRestRequest().find("boundary=");
        if (pos_boundary == std::string::npos)
        {
            std::cout << "ERROR BONDARY NOT FOND\n"; // chould be send response bad requrest; 
            return ;
        }
        size_t pos_rn = dataClient.getRestRequest().find("\r\n", pos_boundary);
        dataClient.setBoundaryRequest(dataClient.getRestRequest().substr(pos_boundary + 9, pos_rn - pos_boundary - 7));
        std::cout << dataClient.getBoundarytSocket() << std::endl;
        size_t pos_Content = dataClient.getRestRequest().find("Content-Length:", 0) + 16;
        if (pos_Content == std::string::npos)
        {
            std::cout << "ERROR BONDARY NOT FOND\n"; // chould be send response bad requrest; 
            return ;
        }
        size_t poss_end = dataClient.getRestRequest().find("\r\n", pos_Content);
        dataClient.setContentLength((ssize_t)std::atof(dataClient.getRestRequest().substr(pos_Content,poss_end - pos_Content).c_str()));
        // std::string k = Header.substr(pos, poss - pos + 1);
        std::cout << dataClient.getContentLength() << std::endl;
    }
    if (tokens[0] == "" ||  (tokens[0] != "GET" && tokens[0] != "POST" && tokens[0] != "DELETE"))
    {
        return;
    }
    dataClient.setHeaderStatus(true);
    // exit(1);
    // std::string kk = k.substr(0, k.size() -2 );
    // }
    // std::cout << "|" << kk << "|" << std::endl;
}

void request::check_Get_Request(Client &dataClient)
{
    std::string line;
    std::string u = "/Users/eelhafia/Desktop/webServer" + dataClient.getUrl();
    std::cout << "|" << u << "|" << std::endl;
    if (access(u.c_str(), R_OK) && dataClient.getUrl() != "/html/html/app-coder.html")
    {
        // error = 404;
        std::string rOK = "HTTP/1.1 404 Not Found\r\nContent-Length: ";
        rsp.sendResponse("/Users/eelhafia/Desktop/webServer/html/404.html", rOK, dataClient.getClientSocket());
        dataClient.setHeaderStatus(false);
        dataClient.resetRestRequest();
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
        dataClient.setTypeRequset("");
        dataClient.setHeaderStatus(false);
        dataClient.setUrl("");
        dataClient.resetRestRequest();
        return;
    }
}


request::request(/* args */)
{
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

void request::download_file(Client &dataClient, ssize_t pos_start)
{
    std::size_t startPos = dataClient.getRestRequest().find("--" + dataClient.getBoundarytSocket(), pos_start);
    if (startPos == std::string::npos)
        return ;
    if(dataClient.getFileName() == "")
    {
        size_t p = dataClient.getRestRequest().find("filename=", startPos); // bad request if not fond
        if (p == std::string::npos)
            std::cout << "not fond \n" ;
        p += 9;
        size_t po = dataClient.getRestRequest().find("\r\n", p); // bad request if not fond
        std::string namefile = dataClient.getRestRequest().substr(p + 1, po - p - 2);
        namefile = "/Users/eelhafia/Desktop/webServer/download/" + namefile;
        std::cout << namefile << std::endl;
        dataClient.setFileName(namefile);
        std::ofstream file(namefile, std::ios::out | std::ios::binary);
        if (!file) {
            std::cerr << "Error opening file for writing" << namefile << std::endl;
            return;
        }
    }
    if (dataClient.getFileName() != "")
    {
        std::string bonadry;

        bonadry = "--" + dataClient.getBoundarytSocket();
        size_t endPos = dataClient.getRestRequest().find(bonadry, startPos + bonadry.length() - 2);
        size_t start = dataClient.getRestRequest().find("\r\n\r\n", startPos);
        if (start != std::string::npos)
            start += 4;
        std::ofstream file(dataClient.getFileName(), std::ios::app | std::ios::binary);
        file.write(dataClient.getRestRequest().c_str() + start, endPos - start - 2);
        file.close();
        size_t pos_next_start = dataClient.getRestRequest().find(bonadry, endPos + bonadry.length() - 4);
        if (pos_next_start != std::string::npos)
        {
            dataClient.setFileName("");
            download_file(dataClient, endPos);
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
            if (i % 3 == 0)
                std::cout << "/";
            else if (i % 3 == 1)
                std::cout << "-";
            else
                std::cout << "\\";
                

        } else {
            std::cout << " ";
        }
    }
    
    std::cout << "] " << percentage << "%";
    std::cout.flush();
    std::cout << "\r";
}

void    request::read_request(Client& dataClient)
{
    char buffer[3001];
    int len_read = 3000;
    std::memset(buffer, 0, sizeof(buffer));
    std::string buf;
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
        dataClient.setRestRequest(buffer, bytesRead);
        if (dataClient.getHeaderStatus() == true)
            dataClient.setReadlen(bytesRead);
        if (!dataClient.getHeaderStatus() && dataClient.getRestRequest().find("\r\n\r\n") != std::string::npos)
            parse_request(dataClient);
        
        std::cout <<"|"<< dataClient.getReadlen() <<"|"<< std::endl;
    
    
        if (dataClient.getReadlen() && dataClient.getTypeRequset() == "POST" && dataClient.getHeaderStatus() == true)
        {
            printLoadingBar((double)(dataClient.getReadlen()) / dataClient.getContentLength() * 100, 40);
            
        }

        std::memset(buffer, 0, sizeof(buffer));
        // if (dataClient.getReadlen() && dataClient.getTypeRequset() == "POST" && dataClient.getReadlen() >= dataClient.getContentLength())
        // {
        //     // close(dataClient.getClientSocket());
        
        //     std::string response1 = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 10 \r\n\r\n donne!!!!!";
        //     send(dataClient.getClientSocket(), response1.c_str(), response1.size(),0);
        //     close(dataClient.getClientSocket());   
        //     dataClient.resetData();
        // }
        // if (Header.find("\r\n\r\n") != std::string::npos && dataClient.getTypeRequset() == "GET")
        //     break;
    }
    // std::ofstream f("tttt.txt");
    // f.write(dataClient.getRestRequest().c_str(), dataClient.getReadlen());
    // return ;
    // if (dataClient.getReadlen() && dataClient.getContentLength() && dataClient.getReadlen() >= dataClient.getContentLength())
    // {
        
    //     close(dataClient.getClientSocket());
    //     write(dataClient.getClientSocket(), "Content-Lenght 4\r\n\rdone" , 23);
    //     dataClient.resetData();
    // }
    
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
