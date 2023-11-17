#include"CGI.hpp"
#include <fstream>
#include <signal.h>



std::string mainCGI(std::string cgi_exe, Client &dataClient) {

    std::string responsesstring;
    std::string sscriptType = "php";
    try {
        Web_Secript_Setter Web_Secript_Setter(cgi_exe, sscriptType, dataClient);

        if (dataClient.getTypeRequset() == "POST") 
        {
          size_t posBady = dataClient.getRestRequest().find("\r\n\r\n");
          std::string postData = dataClient.getRestRequest().substr(posBady + 4, dataClient.getContentLength() - posBady - 4);
          Web_Secript_Setter.body = postData;
          Web_Secript_Setter.posbody = posBady + 4;
        }
        Web_Secript_Setter.executionCGI();
        Web_Secript_Setter.dataClient = dataClient;
        int readEnd = Web_Secript_Setter.ReadValue();
        sleep(1);
        
        char c;
        int p = 0;
        clock_t time = clock();
        while((p = read(readEnd , &c, 1)))
        {
          if (p == 0 || (clock() - time > 3000))
          {
            dataClient.error = 500;
            Web_Secript_Setter.close_pipes();
            kill(Web_Secript_Setter.pid, SIGKILL);
            return "s";
          }
          if (p < 0)
            break;
          responsesstring += c;
        }
        Web_Secript_Setter.close_pipes();
     } catch (const char* error) {
        std::cerr << "Error: " << error << std::endl;
      }  
    std::stringstream m;
    m << responsesstring.size() - responsesstring.find("\r\n\r\n") - 4;
    std::string res = "HTTP/1.1 200 OK\r\nContent-Length: " + m.str() +"\r\n"+ responsesstring;
    return res;
}
