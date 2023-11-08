#include"CGI.hpp"
#include <fstream>
#include <signal.h>



std::string mainCGI(Client &dataClient) {

    std::string responsesstring;
    std::string spath ="CGI/hello_script.php";
    std::string sfile = "CGI/hello_script.php";
    std::string sscriptType = "php";
    try {
        CGISettler cgiSettler(spath, sfile, sscriptType, dataClient);

        if (dataClient.getTypeRequset() == "POST") 
        {
          cgiSettler.CgiEnv(dataClient);
          size_t posBady = dataClient.getRestRequest().find("\r\n\r\n");
          std::string postData = dataClient.getRestRequest().substr(posBady + 4, dataClient.getContentLength());
          cgiSettler.body = postData;
        }
        cgiSettler.executionCGI();
        cgiSettler.dataClient = dataClient;
        std::string postDate = "my data";
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
