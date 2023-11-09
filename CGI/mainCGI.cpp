#include"CGI.hpp"
#include <fstream>
#include <signal.h>



std::string mainCGI(std::string cgi_exe, Client &dataClient) {

    std::string responsesstring;
    std::string sscriptType = "php";
    try {
        CGISettler cgiSettler(cgi_exe, sscriptType, dataClient);

        if (dataClient.getTypeRequset() == "POST") 
        {
          size_t posBady = dataClient.getRestRequest().find("\r\n\r\n");
          std::string postData = dataClient.getRestRequest().substr(posBady + 4, dataClient.getContentLength() - posBady - 4);
          cgiSettler.body = postData;
          cgiSettler.posbody = posBady + 4;
        }
        cgiSettler.executionCGI();
        cgiSettler.dataClient = dataClient;
        int readEnd = cgiSettler.getReadEnd();
        sleep(1);
        char c;
        
        int p = 0;
        while((p = read(readEnd , &c, 1)))
        {
          if (p == 0)
          {
            dataClient.error = 500;
            kill(cgiSettler.pid, SIGKILL);
            cgiSettler.close_pipes();
            return "s";
          }
          if (p < 0)
            break;
          responsesstring += c;
        }
        cgiSettler.close_pipes();
     } catch (const char* error) {
        std::cerr << "Error: " << error << std::endl;
      }  
    std::string res = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responsesstring.size() - responsesstring.find("\r\n\r\n") - 4) +"\r\n"+ responsesstring;
    return res;
}
