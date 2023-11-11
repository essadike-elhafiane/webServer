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
        }
        else
        {            
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
    return 0;
}

void request::parse_request(Client& dataClient)
{
    size_t poss = dataClient.getRestRequest().find("\r\n\r\n");
    if (poss == std::string::npos)
    {
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
    size_t postype = dataClient.getRestRequest().find("Transfer-Encoding:");
    if (postype != std::string::npos)
        dataClient.TransferEncoding = dataClient.getRestRequest().substr(postype + 19,dataClient.getRestRequest().find("\r\n", postype) - postype - 19);
    if (tokens[2].substr(0,tokens[2].find("\r\n")) != "HTTP/1.1" || tokens[1].empty() || tokens[0] == "")
    {
        dataClient.error = 400;
        return ;
    }
    dataClient.setTypeRequset(tokens[0]);
    dataClient.setUrl(tokens[1]);
    if (tokens[0] == "POST" && dataClient.TransferEncoding != "chunked")
    {
        size_t pos_boundary = dataClient.getRestRequest().find("boundary=");
        if (pos_boundary == std::string::npos)
        {
            dataClient.error = 400;
            return ;
        }
        size_t pos_rn = dataClient.getRestRequest().find("\r\n", pos_boundary);
        if (dataClient.TransferEncoding == "chunked")
            dataClient.setBoundaryRequest(dataClient.getRestRequest().substr(pos_boundary + 9, pos_rn - pos_boundary - 9));
        else
            dataClient.setBoundaryRequest(dataClient.getRestRequest().substr(pos_boundary + 9, pos_rn - pos_boundary - 7));
    }
    if (tokens[0] == "POST")
    {
        size_t pos_Content = dataClient.getRestRequest().find("Content-Length:", 0) + 16;
        if (pos_Content == std::string::npos)
        {
            dataClient.error = 400;
            return ;
        }
        size_t poss_end = dataClient.getRestRequest().find("\r\n", pos_Content);
        dataClient.setContentLength((ssize_t)std::atof(dataClient.getRestRequest().substr(pos_Content,poss_end - pos_Content).c_str()));
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
}


request::request(/* args */)
{
}

request::~request()
{
}

int request::parseChunck(Client& dataClient, size_t pos)
{
    size_t posf = dataClient.getRestRequest().find("\r\n\r\n", pos);
    size_t poss = dataClient.getRestRequest().find("\r\n", posf + 4);
    if (poss == std::string::npos)
        return (dataClient.error = 400);
    size_t lenRead = 0;
    std::string len = dataClient.getRestRequest().substr(posf + 4, poss - posf - 4);
    std::stringstream f;
    f << len;
    int ll;
    f >> std::hex >> ll;
    if (ll == 0)
        return(dataClient.error = 204, 1);
    lenRead += ll;
    dataClient.setBoundaryRequest(dataClient.getRestRequest().substr(poss + 4, dataClient.getRestRequest().find("\r\n", poss + 4) - poss - 4));
    dataClient.getRestRequest().erase(posf + 4, poss - posf - 2);
    size_t startpos = posf + 4 + ll;
    while (startpos != std::string::npos)
    {
        if (dataClient.getRestRequest()[startpos] != '\r')
            return (dataClient.error = 400);
        size_t endPos = dataClient.getRestRequest().find("\r\n", startpos + 2);
        if (endPos ==  std::string::npos)
            break;
        len = dataClient.getRestRequest().substr(startpos + 2, endPos - startpos - 2);
        dataClient.getRestRequest().erase(startpos, endPos + 2 - startpos);
        f.clear();
        f << len;
        f >> std::hex >> ll;
        f << ll;
        lenRead += ll;
        startpos = startpos + ll;
    }
    dataClient.clearReadlen();
    dataClient.setReadlen(lenRead);
    return 0;
}

int request::download_file(Client &dataClient, ssize_t pos_start)
{
    std::size_t startPos = dataClient.getRestRequest().find("--" + dataClient.getBoundarytSocket(), pos_start);
    if (startPos == std::string::npos)
        return (dataClient.error = 400, 1);
    if(dataClient.getFileName() == "")
    {
        size_t p = dataClient.getRestRequest().find("filename=\"", startPos);
        size_t po = dataClient.getRestRequest().find("\r\n", p);
        if (po == std::string::npos && po != std::string::npos)
            return (dataClient.error = 400, 1); // bad request if not fond
        std::string namefile;
        if (p != std::string::npos && po != std::string::npos)
        {
            p += 9;
            namefile = dataClient.getRestRequest().substr(p + 1, po - p - 2);
        }
        size_t posname = dataClient.getRestRequest().find("name=\"", startPos);
        size_t endPosname;
        if (po != std::string::npos)
            endPosname = dataClient.getRestRequest().find("\";", posname);
        else
            endPosname = dataClient.getRestRequest().find("\"\r\n", posname);
        std::string name;
        if (posname != std::string::npos && endPosname != std::string::npos)
           name = dataClient.getRestRequest().substr(posname + 6, endPosname - posname - 6);
        if (namefile.empty())
            namefile = dataClient.getUrl() + "/" + name;
        else
            namefile = dataClient.getUrl() + "/" + namefile;
        if (namefile.empty() && name.empty())
            return (dataClient.error = 204, 1);
        dataClient.setFileName(namefile);
        std::ofstream file(namefile, std::ios::out | std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error opening file for writing" << namefile << std::endl;
            return (dataClient.error = 204, 1);
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
        if (endPos - start - 2 == 0)
            return (dataClient.error = 204, 1);
        std::ofstream file(dataClient.getFileName(), std::ios::app | std::ios::binary);
        file.write(dataClient.getRestRequest().c_str() + start, endPos - start - 2);
        file.close();
        size_t pos_next_start = dataClient.getRestRequest().find(bonadry, endPos + bonadry.length() - 4);
        if (pos_next_start != std::string::npos && !dataClient.getBoundarytSocket().empty())
        {
            dataClient.setFileName("");
            download_file(dataClient, endPos);
        }
    }
    return 0;
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
        {
           parse_request(dataClient);
           if (checkAllowedMethods(dataClient))
                return ;
        }
        if (dataClient.getContentLength() > dataClient.configData.client_max_body_size)
            dataClient.error = 413;
    
        if (dataClient.getReadlen() && dataClient.getTypeRequset() == "POST" && dataClient.getHeaderStatus() == true)
            printLoadingBar((double)(dataClient.getReadlen()) / dataClient.getContentLength() * 100, 40);
 
        std::memset(buffer, 0, sizeof(buffer));
        if (dataClient.error)
            return ;
    }
    // if (!dataClient.getHeaderStatus())
    //     dataClient.error = 408;
}

void request::delete_request(Client& dataClient)
{
    std::string filename;
    if (dataClient.error)
        return ;
    filename = dataClient.getUrl();
    if (dataClient.getUrl() == "")
        return ;
    int result = std::remove(filename.c_str());
    if (result == 0) {
        dataClient.setUrl("html/delete.html");
    } else {
        dataClient.setUrl("html/not_delete.html");
    }
}
