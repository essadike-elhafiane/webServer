/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eelhafia <eelhafia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 21:28:36 by eelhafia          #+#    #+#             */
/*   Updated: 2023/10/03 22:51:28 by eelhafia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/webserv.hpp"
#include "server/server.hpp"
#include "socketClient/socketClient.hpp"
#include "request/request.hpp"

int main() {
    server a("server1");
    a.runServer(5, 3000);

    std::string requests;
    // Accept client connections and respond
    // char b[30000];
    while (true) {
        socketClient client;
        client.setUpAndAccept(a);
        request request;
        request.receiveRequest(client);
        // int len = recv(client.getClientSocket(), b, 30000 - 1, 0);
        // b[len] = '\n';
        // b[len +1] = '\0';
        // requests = b;
        if (request.getUrl()== "/html/html/compte.html")
        {
            std::cout << requests << std::endl;
            std::string rOK = "HTTP/1.1 200 OK\r\nContent-Length: ";
            std::ifstream r("html/compte.html");
            if (!r.is_open())
                exit(1);
            std::string response;
            std::string line; 
            while (!std::getline(r, line).fail())
                response = response + line + '\n';
            
            rOK = rOK + std::to_string(response.length()) + "\r\n\r\n" + response;
            std::cout << std::endl << std::endl;
            std::cout << rOK << std::endl;
            if (send(client.getClientSocket(), rOK.c_str(), rOK.length(), 0) < 0)
            {
                std::cerr << "Failed to send response." << std::endl;
                close(client.getClientSocket());
                continue;
            }
            close(client.getClientSocket());
            // std::cout << std::endl << "|" << requests.substr(4, 17) << "|" << std::endl << std::endl;
            continue;
        }
        if (request.getUrl() == "html/app-coder.html" || request.getUrl() == "/html/html/app-coder.html")
        {
            // std::cout << requests << std::endl;
            std::string rOK = "HTTP/1.1 200 OK\r\nContent-Length: ";
            std::ifstream r("html/app-coder.html");
            if (!r.is_open())
                exit(1);
            std::string response;
            std::string line; 
            while (!std::getline(r, line).fail())
                response = response + line + '\n';
            
            rOK = rOK + std::to_string(response.length()) + "\r\n\r\n" + response;
            std::cout << std::endl << std::endl;
            std::cout << rOK << std::endl;
            if (send(client.getClientSocket(), rOK.c_str(), rOK.length(), 0) < 0)
            {
                std::cerr << "Failed to send response." << std::endl;
                close(client.getClientSocket());
                continue;
            }
            
            close(client.getClientSocket());
            // std::cout << std::endl << "|" << requests.substr(4, 25) << "|" << std::endl << std::endl;
            continue;
        }
        // std::cout << std::endl << requests.substr(4, 12) << std::endl;
        // std::string message = "Hello from server!";
        // std::strcpy(buffer, message.c_str());
        // write(client.getClientSocket(), buffer, message.length());
        // std::cout << requests << std::endl;
        if (request.getUrl() == "/" || request.getUrl() == "/html/conection.html")
        {
            std::string rOK = "HTTP/1.1 200 OK\r\nContent-Length: ";
            std::ifstream r("html/file.html");
            if (!r.is_open())
                exit(1);
                
            std::string response;
            std::string line; 
            while (!std::getline(r, line).fail())
                response = response + line + '\n';
            
            rOK = rOK + std::to_string(response.length()) + "\r\n\r\n" + response;
            std::cout << std::endl << std::endl;
            // std::cout << rOK << std::endl;
            if (send(client.getClientSocket(), rOK.c_str(), rOK.length(), 0) < 0)
            {
                std::cerr << "Failed to send response." << std::endl;
                close(client.getClientSocket());
                continue;
            }
        }
        
        close(client.getClientSocket());
        // exit(1);
    }

    close(a.getServerSocket());
    return 0;
}