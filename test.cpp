#include <iostream>
#include <string>
#include <map>

// int solve(int a)
// {
//     int result = 0;

//     if (a < 0)
//         return 0;
//     if (a == 0)
//         return 1;
//     for (size_t i = 1; i < 7; i++)
//     {
//         result += solve(a - i);
//         if (result < 0)
//             break;
//     }
//     return result;
// }
void 
int main()
{
    atexit(f);
    std::map<std::string, int> m;

    m["ahmed"] = 1;
    m["mohamed"] = 2;
    
    m.erase("ahmed");
    std::cout << m.find("ahmed")->second << std::endl;
    // int a;
    // std::cin >> a;

    // std::cout << solve(a);
}

// int len = recv(client.getClientSocket(), b, 30000 - 1, 0);
        // b[len] = '\n';
        // b[len +1] = '\0';
        // requests = b;
        // if (request.getUrl()== "/html/html/compte.html" || request.getUrl()== "/html/compte.html")
        // {
        //     std::cout << requests << std::endl;
        //     std::string rOK = "HTTP/1.1 200 OK\r\nContent-Length: ";
        //     std::ifstream r("html/compte.html");
        //     if (!r.is_open())
        //         exit(1);
        //     std::string response;
        //     std::string line; 
        //     while (!std::getline(r, line).fail())
        //         response = response + line + '\n';
            
        //     rOK = rOK + std::to_string(response.length()) + "\r\n\r\n" + response;
        //     std::cout << std::endl << std::endl;
        //     std::cout << rOK << std::endl;
        //     if (send(client.getClientSocket(), rOK.c_str(), rOK.length(), 0) < 0)
        //     {
        //         std::cerr << "Failed to send response." << std::endl;
        //         close(client.getClientSocket());
        //         continue;
        //     }
        //     close(client.getClientSocket());
        //     // std::cout << std::endl << "|" << requests.substr(4, 17) << "|" << std::endl << std::endl;
        //     continue;
        // }
        // if (request.getUrl() == "/html/app-coder.html" || request.getUrl() == "/html/html/app-coder.html")
        // {
        //     // std::cout << requests << std::endl;
        //     std::string rOK = "HTTP/1.1 200 OK\r\nContent-Length: ";
        //     std::ifstream r("html/app-coder.html");
        //     if (!r.is_open())
        //         exit(1);
        //     std::string response;
        //     std::string line; 
        //     while (!std::getline(r, line).fail())
        //         response = response + line + '\n';
            
        //     rOK = rOK + std::to_string(response.length()) + "\r\n\r\n" + response;
        //     std::cout << std::endl << std::endl;
        //     std::cout << rOK << std::endl;
        //     if (send(client.getClientSocket(), rOK.c_str(), rOK.length(), 0) < 0)
        //     {
        //         std::cerr << "Failed to send response." << std::endl;
        //         close(client.getClientSocket());
        //         continue;
        //     }
            
        //     close(client.getClientSocket());
        //     // std::cout << std::endl << "|" << requests.substr(4, 25) << "|" << std::endl << std::endl;
        //     continue;
        // }
        // std::cout << std::endl << requests.substr(4, 12) << std::endl;
        // std::string message = "Hello from server!";
        // std::strcpy(buffer, message.c_str());
        // write(client.getClientSocket(), buffer, message.length());
        // std::cout << requests << std::endl;
        // if (request.getUrl() == "/" || request.getUrl() == "/html/conection.html")
        // {
        //     std::string rOK = "HTTP/1.1 200 OK\r\nContent-Length: ";
        //     std::ifstream r("html/file.html");
        //     if (!r.is_open())
        //         exit(1);
                
        //     std::string response;
        //     std::string line; 
        //     while (!std::getline(r, line).fail())
        //         response = response + line + '\n';
            
        //     rOK = rOK + std::to_string(response.length()) + "\r\n\r\n" + response;
        //     std::cout << std::endl << std::endl;
        //     // std::cout << rOK << std::endl;
        //     if (send(client.getClientSocket(), rOK.c_str(), rOK.length(), 0) < 0)
        //     {
        //         std::cerr << "Failed to send response." << std::endl;
        //         close(client.getClientSocket());
        //         continue;
        //     }
        // }
        