#include <iostream>
#include <cstring>
#include<vector>

int main() {
    std::vector<std::string> v;
    char str[] = "{}hello||myfriendhh}";
    char *token;
    std::string tmp = "server";
     std::string tmp1 = "server1";
    if (tmp == tmp1)
        exit(0);
    std::cout << tmp.length() ;
    exit(0);
    token =  std::strtok(str ," ");
    while(token != nullptr)
    {
        tmp =(token);
        v.push_back(tmp);
        token =  std::strtok(nullptr ," \n\t\r");

    }
    for (std::vector<std::string>::iterator itr = v.begin(); itr != v.end(); itr++)
    {
        std::cout << "|"<< *itr << "|" <<std::endl;
    }
    
    // std::cout << token;
    return (0);
}