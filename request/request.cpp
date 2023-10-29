#include "request.hpp"

int isdigits(std::string Host)
{
    for (size_t i = 0; Host[i]; i++)
    {
        if(!isdigit(Host[i]))
            return 0;
    }
    return 1;
}

int checkIsIp(std::string host)
{
    for (size_t i = 0; host[i]; i++)
    {
       if (!isdigit(host[i]) && host[i] != '.')
            return 0;
    }
    return 1;
}

int    getHost(std::string& requests,size_t posHost, Client& dataClient)
{
    size_t endPosHost = requests.find("\r\n", posHost);
    std::string Host = requests.substr(posHost + 6, endPosHost - posHost - 6);
    size_t posPort = Host.find(":");
    if(!isdigits(Host))
    {
        if (posPort == std::string::npos)
        {
            dataClient.HostName = Host;
            dataClient.port = 0;
            return 0;
        }
    }
    dataClient.HostName = Host.substr(0, posPort);
    dataClient.port = std::atoi(Host.substr(posPort + 1, Host.size() - posPort - 1).c_str());
    return 0;
}

int checkValidRequest(std::string &requests, size_t poss, Client& dataClient)
{
    std::stringstream s(requests.substr(0, poss + 2));
    std::string line;
    int i = 0;
    
    while (!std::getline(s, line, '\n').fail())
    {
        if(i == 0)
        {
            int count = std::count(line.begin(), line.end(), ' ');
            if (count != 2)
                return 1;    
            if (line[line.size() - 1] != '\r')
                return 1;   
            i++; 
            //std::cout<< "1111\n"; 
            std::cout<< line<< "\n";
        }
        else
        {            
            std::cout<< line<< "\n";
            if (line[line.size() - 1] != '\r')
                return 1;
            size_t posSpace = line.find(" ");
            if (posSpace == std::string::npos)
                return 1;
            if (line.substr(0, posSpace)[line.substr(0, posSpace).size() - 1] != ':')
                return 1;
        }
    }
    size_t posHost = requests.find("Host:");
    if (posHost == std::string::npos)
        return 1;
    getHost(requests, posHost, dataClient);
    if (dataClient.HostName.empty())
        return 1;
    std::cout << "|" << dataClient.HostName << "|" << std::endl;
    if (!checkIsIp(dataClient.HostName))
    {
        size_t posServerData = 0;
        while (posServerData < dataClient.dataServers.size())
        {
            if (dataClient.HostName == dataClient.dataServers[posServerData].server_name)
                break;
            posServerData++;
        }
        if (posServerData == dataClient.dataServers.size())
            return 0;
        dataClient.configData = dataClient.dataServers[posServerData];
    }
    //std::cout<< "|" << dataClient.HostName << "|" << dataClient.port << "|" << dataClient.getTypeRequset() << "\n";
    return 0;
}

void request::parse_request(Client& dataClient)
{
    size_t poss = dataClient.getRestRequest().find("\r\n\r\n");
    if (poss == std::string::npos)
    {
        //std::cout<< "not\n";
        dataClient.error = 400;
        return ;
    }
    long int p = dataClient.getRestRequest().length() - poss - 4;
    if (p < 0)
        p = 0;
    dataClient.setReadlen(p);
    if (checkValidRequest(dataClient.getRestRequest(), poss, dataClient))
    { 
        dataClient.error = 400;
        return ;
    }
    std::stringstream ss(dataClient.getRestRequest().substr(0, poss));
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
        if (tokens.size() > 2)
            break;
    }
    if (tokens[2].substr(0,tokens[2].find("\r\n")) != "HTTP/1.1" || tokens[1].empty() || tokens[0] == "")
    {
        dataClient.error = 400;
        //std::cout<< "Error http1.1 not fond\n";
        return ;
    }
    // //std::cout<< "hhh |" << dataClient.getRestRequest() << "|" << std::endl;
    dataClient.setTypeRequset(tokens[0]);
    dataClient.setUrl(tokens[1]);
    if (tokens[0] == "POST")
    {
        size_t pos_boundary = dataClient.getRestRequest().find("boundary=");
        if (pos_boundary == std::string::npos)
        {
            dataClient.error = 400;
            return ;
        }
        size_t pos_rn = dataClient.getRestRequest().find("\r\n", pos_boundary);
        dataClient.setBoundaryRequest(dataClient.getRestRequest().substr(pos_boundary + 9, pos_rn - pos_boundary - 7));
        // //std::cout<< dataClient.getBoundarytSocket() << std::endl;
        size_t pos_Content = dataClient.getRestRequest().find("Content-Length:", 0) + 16;
        if (pos_Content == std::string::npos)
        {
            dataClient.error = 400;
            return ;
        }
        size_t poss_end = dataClient.getRestRequest().find("\r\n", pos_Content);
        dataClient.setContentLength((ssize_t)std::atof(dataClient.getRestRequest().substr(pos_Content,poss_end - pos_Content).c_str()));
        
        // std::string k = Header.substr(pos, poss - pos + 1);
        // //std::cout<< dataClient.getContentLength() << std::endl;
    }
    size_t pos = dataClient.getRestRequest().find("Connection:");
    if (pos != std::string::npos)
    {
        size_t Endpos = dataClient.getRestRequest().find("\r\n", pos);
        dataClient.connection = dataClient.getRestRequest().substr(pos + 12, Endpos - pos - 12);
    }
    else
        dataClient.connection = "";
    if (tokens[0] == "" ||  (tokens[0] != "GET" && tokens[0] != "POST" && tokens[0] != "DELETE"))
    {
        dataClient.error = 400;
        return ;
    }
    dataClient.setHeaderStatus(true);
    // //exit(1);
    // std::string kk = k.substr(0, k.size() -2 );
    // }
    // //std::cout<< "|" << kk << "|" << std::endl;
}

void request::check_Get_Request(Client &dataClient)
{
    if (dataClient.getUrl() == "/")
    {
        size_t i;
        for (i = 0; i < dataClient.configData.pages.size(); i++)
            if (dataClient.configData.pages[i].path == "/")
                break;
        if (i == dataClient.configData.pages.size())
        {
            dataClient.error = 404;
            //std::cout<< "{{-1}}\n";
            return ;
        }
        dataClient.setUrl(dataClient.configData.pages[i].root + dataClient.configData.pages[i].index);
        //std::cout<< dataClient.getUrl() << std::endl;
        return ;
    }
    else
    {
        
        size_t pos = dataClient.getUrl().find("/", 1);
        if (pos == std::string::npos)
        {
            size_t i;
            for (i = 0; i < dataClient.configData.pages.size(); i++)
                if (dataClient.configData.pages[i].path == "/")
                    break;
            if (i == dataClient.configData.pages.size())
            {
                //std::cout<< "{{00}}\n";
                dataClient.error = 404;
                return ;
            }
            dataClient.setUrl(dataClient.configData.pages[i].root + dataClient.getUrl());
            // //std::cout<< "!!!" << dataClient.getUrl() << "!!!" << std::endl;
            return ;
        }
        std::string nameLocation = dataClient.getUrl().substr(0, pos);
        size_t i;
        for (i = 0; i < dataClient.configData.pages.size(); i++)
        {   
            if (dataClient.configData.pages[i].path == nameLocation)
                break;
            //std::cout<< dataClient.configData.pages[i].path;
        }
       if (i == dataClient.configData.pages.size())
        {
            //std::coutt<< "{{1}}\n";
            dataClient.error = 404;
            return ;
        }
        dataClient.setUrl(dataClient.configData.pages[i].root + dataClient.getUrl());
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

int request::download_file(Client &dataClient, ssize_t pos_start)
{
    std::size_t startPos = dataClient.getRestRequest().find("--" + dataClient.getBoundarytSocket(), pos_start);
    if (startPos == std::string::npos)
        return (dataClient.error = 400, 1);
    if(dataClient.getFileName() == "")
    {
        size_t p = dataClient.getRestRequest().find("filename=", startPos); // bad request if not fond
        if (p == std::string::npos)
            return (dataClient.error = 400, 1);
        p += 9;
        size_t po = dataClient.getRestRequest().find("\r\n", p);
        if (po == std::string::npos)
            return (dataClient.error = 400, 1); // bad request if not fond
        std::string namefile = dataClient.getRestRequest().substr(p + 1, po - p - 2);
        size_t i = 0;
        for (i = 0; i < dataClient.configData.pages.size() ; i++)
        {
            if (dataClient.configData.pages[i].path == "/upload")
                break;
        }
        namefile = dataClient.configData.pages[i].root + namefile;
        dataClient.setFileName(namefile);
        std::ofstream file(namefile, std::ios::out | std::ios::binary);
        if (!file) {
            std::cerr << "Error opening file for writing" << namefile << std::endl;
            return (dataClient.error = 400, 1);
        }
    }
    if (dataClient.getFileName() != "")
    {
        std::string bonadry;

        bonadry = "--" + dataClient.getBoundarytSocket();
        size_t endPos = dataClient.getRestRequest().find(bonadry, startPos + bonadry.length() - 2);
        if (endPos == std::string::npos)
        {
            endPos = dataClient.getRestRequest().find(bonadry.substr(0, bonadry.length() - 2) + "--");
            if (endPos == std::string::npos)
                return (dataClient.error = 400, 1);
        }
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
    return 0;
    // /goinfre/eelhafia/download/
}

void printLoadingBar(int percentage, int barWidth) {
    std::cout<< "Loading: [";
    
    int progress = (percentage * barWidth) / 100;
    
    for (int i = 0; i < barWidth; ++i) {
        if (i < progress) {
            std::cout<< "=";
        } else if (i == progress) {
            if (i % 3 == 0)
                std::cout<< "/";
            else if (i % 3 == 1)
                std::cout<< "-";
            else
                std::cout<< "\\";
                

        } else {
            std::cout<< " ";
        }
    }
    
    std::cout<< "] " << percentage << "%";
    std::cout.flush();
    std::cout<< "\r";
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
            std::cout<< "connection closed by client " << dataClient.getClientSocket() << "\n";  
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
        if (dataClient.getContentLength() > dataClient.configData.client_max_body_size)
        {
            std::cout << dataClient.getContentLength() << "|" << dataClient.configData.client_max_body_size << std::endl;
            dataClient.error = 413;
        }
        // //std::cout<<"|"<< dataClient.getReadlen() <<"|"<< std::endl;
    
    
        if (dataClient.getReadlen() && dataClient.getTypeRequset() == "POST" && dataClient.getHeaderStatus() == true)
            printLoadingBar((double)(dataClient.getReadlen()) / dataClient.getContentLength() * 100, 40);
 

        std::memset(buffer, 0, sizeof(buffer));
        if (dataClient.error)
            return ;
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
    if (!dataClient.getHeaderStatus())
        dataClient.error = 408;
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
    size_t i = 0;
    for (i = 0; i < dataClient.configData.pages.size() ; i++)
    {
        if (dataClient.configData.pages[i].path == "/upload")
            break;
    }
    filename = dataClient.configData.pages[i].root + dataClient.getUrl();
    if (dataClient.getUrl() == "")
        return ;
    int result = std::remove(filename.c_str());
    if (result == 0) {
        // std::string response1 = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
        dataClient.setUrl("/Users/eelhafia/Desktop/webServer/html/delete.html");
        // rsp.sendResponse("/Users/eelhafia/Desktop/webServer/html/delete.html", response1, dataClient.getClientSocket(), dataClient);
        // printf("File deleted successfully.");
        // dataClient.resetData();
    } else {
        dataClient.setUrl("/Users/eelhafia/Desktop/webServer/html/not_delete.html");
        // rsp.sendResponse("/Users/eelhafia/Desktop/webServer/html/not_delete.html", response1, dataClient.getClientSocket(), dataClient);
        // printf("Failed to delete the file.\n");
        // dataClient.resetData();
    }
}
