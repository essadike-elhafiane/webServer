#include"CGI.hpp"
#include <fstream>

std::string mainCGI(std::string urlCgi, int clientSocket, Client &dataClient) {

     HttpRequest request;
    //  std::string 
    (void) clientSocket;
    //  Clientt Clientt;
    std::cout << " ||||"<< urlCgi << std::endl;

    request.request_data = "example request data";
    //dataClient.getRestRequest().find(wjtjkw);

  
    // std::cout<< dataClient.getContentLength()<<std::endl;
    // std::cout<< dataClient.getTypeRequset()<<std::endl;

    //std::string path = "/Users/edraidry/Desktop/webServer/CGIfix/test.py";
    std::string path ="/Users/eelhafia/Desktop/webServer/CGI/hello_script.php";
    //std::string file = "/Users/eelhafia/Desktop/webServer/CGIfix/test.py";
    // std:: cout<<file<<std::endl;
    std::string file = "/Users/eelhafia/Desktop/webServer/CGI/hello_script.php";
    std::string responsesstring;
     try {

        std::string scriptType = "php";
    
        CGISettler resolver(path, file, scriptType, request,dataClient);
        resolver.executionCGI();
        resolver.dataClient = dataClient;
        wait(0);
        
        // resolver.processResponse();

   
       
    
        //  std::cout << "Response data: " << Clientt.response_data << std::endl;


        std::string postDate = "my data";
        int readEnd = resolver.getReadEnd();
        //int writeEnd = resolver.getWriteEnd();
    
       // write(writeEnd, postDate.c_str() ,postDate.size());

        char c;
        while(read(readEnd ,&c,1))
        {
          responsesstring += c;
           
        }
        response resp;
        std::string result;
        // std::cout << responsesstring << std::endl;
        // size_t pos = responsesstring.find("\r\n\r\n");
        // std::string header = responsesstring.substr(0, pos);
        // std::ofstream  f("cgi.txt");
        // f << responsesstring.c_str() + pos + 4;

        
        
        // result = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<title>Document</title>\
        //         </head>\n<body>\n<h1>" + responsesstring + "</h1></body>";
        // std::cout<<result<< " " << clientSocket << std::endl;
        // std::cout << "Read end: " << readEnd << std::endl;
        // std::cout << "Write end: " << writeEnd << std::endl;


        // resp.(clientSocket, responsesstring);


        // char* const* env = resolver.getEnv();

     } catch (const char* error) {
        std::cerr << "Error: " << error << std::endl;
      }
        
    std::string res = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responsesstring.size() - responsesstring.find("\r\n\r\n") - 4) +"\r\n"+ responsesstring;
    std::cout << res;
    // dataClient.cgi.append(res);
    return res;
}
