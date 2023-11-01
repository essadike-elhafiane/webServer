#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096

std::string getContentType(const std::string& filename) {
    std::string extension;
    std::size_t dotIndex = filename.rfind('.');
    if (dotIndex != std::string::npos) {
        extension = filename.substr(dotIndex + 1);
    }

    if (extension == "html" || extension == "htm") {
        return "text/html";
    } else if (extension == "txt") {
        return "text/plain";
    } else if (extension == "css") {
        return "text/css";
    } else if (extension == "js") {
        return "application/javascript";
    } else if (extension == "jpg" || extension == "jpeg") {
        return "image/jpeg";
    } else if (extension == "png") {
        return "image/png";
    } else if (extension == "gif") {
        return "image/gif";
    } else if (extension == "pdf") {
        return "application/pdf";
    } else {
        return "application/octet-stream";
    }
}

std::string generateDirectoryListing(const std::string& directory) {
    std::ostringstream listingStream;
    listingStream << "<html><head><title>Directory Listing</title></head><body>";
    listingStream << "<h1>Directory Listing</h1>";
    listingStream << "<ul>";

    DIR* dir = opendir(directory.c_str());
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            std::string filename = entry->d_name;
            if (filename != "." && filename != "..") {
                std::string filepath = directory + "/" + filename;
                struct stat filestat;
                if (stat(filepath.c_str(), &filestat) == 0) {
                    listingStream << "<li><a href=\"" << filename << "\">" << filename << "</a></li>";
                }
            }
        }
        closedir(dir);
    }

    listingStream << "</ul></body></html>";
    return listingStream.str();
}

void sendResponse(int sockfd, const std::string& response) {
    if (send(sockfd, response.c_str(), response.length(), 0) == -1) {
        std::cerr << "Error occurred while sending the HTTP response" << std::endl;
    }
}

void handleRequest(int sockfd) {
    // ... Parse the request and extract the requested directory ...

    std::string directory = "/goinfre/mserrouk";

    std::ostringstream responseStream;
    responseStream << "HTTP/1.1 200 OK\r\n";
    responseStream << "Connection: close\r\n";
    responseStream << "Content-Type: text/html\r\n";
    responseStream << "\r\n";

    std::string response = responseStream.str();
    sendResponse(sockfd, response);

    std::string directoryListing = generateDirectoryListing(directory);
    sendResponse(sockfd, directoryListing);

    // Close the connection
    close(sockfd);
}

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error occurred while creating a socket" << std::endl;
        return 1;
    }
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        std::cerr << "Failed to set socket options." << std::endl;
        return 1;
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(8080);

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error occurred while binding the socket" << std::endl;
        return 1;
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (true) {
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        if (newsockfd < 0) {
            std::cerr << "Error occurred while accepting a connection" << std::endl;
            continue;
        }

        handleRequest(newsockfd);
    }

    close(sockfd);

    return 0;
}
