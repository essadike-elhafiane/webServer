#include"CGI.hpp"
#include <fstream>
#include <signal.h>



std::string mainCGI(std::string urlCgi, int clientSocket, Client &dataClient) {


    HttpRequest request;
  
    std::cout<<urlCgi<<std::endl;
    (void) clientSocket;
  
    std::string spath ="/Users/edraidry/Desktop/webServer/CGI/hello_script.php";
    std::string sfile = "/Users/edraidry/Desktop/webServer/CGI/hello_script.php";
    std::string sscriptType = "php";
    CGISettler cgiSettler(spath, sfile, sscriptType, dataClient);

    if (dataClient.getTypeRequset() == "POST") 
    {
      cgiSettler.CgiEnv(dataClient);
      size_t posBady = dataClient.getRestRequest().find("\r\n\r\n");
      std::string postData = dataClient.getRestRequest().substr(posBady + 4, dataClient.getContentLength());
      cgiSettler.body = postData;
      std::cout<< " ======================>"<<std::endl;
    //   std::cout<< " ======================>\n"<< postData;
      std::cout<< " ======================>"<<std::endl;
    }

    request.request_data = "fix"; ;
   

    //std::string path = "/Users/edraidry/Desktop/fixweb/CGIfix/test.py";
    std::string path ="/Users/edraidry/Desktop/webServer/CGI/hello_script.php";
   
    std::string file = "/Users/edraidry/Desktop/webServer/CGI/hello_script.php";
    std::string responsesstring;
     try {

        std::string scriptType = "php";
        std::cout << "sdgsdgsdgsdgsdgs\n";

        cgiSettler.executionCGI();
        cgiSettler.dataClient = dataClient;
        // wait(0);
        std::cout << "sdgsdgsdgsdgsdgs------------------\n";
        std::string postDate = "my data";
        int readEnd = cgiSettler.getReadEnd();
        sleep(1);
        char c;
        
        int p = 0;
        while((p = read(readEnd , &c, 1)))
        {
            std::cout << "dfhdhfdhdfhdfhdfhdfhd\n";

          if (p <= 0)
          {
            std::cout << "dfhdhfdhdfhdfhdfhdfhd-------------------------------\n";
            dataClient.error = 500;
            kill(cgiSettler.pid, SIGKILL);
            cgiSettler.close_pipes();
            return "s";
          }
          responsesstring += c;
        }
        //std::cout<< responsesstring << std::endl;
        cgiSettler.close_pipes();
     } catch (const char* error) {
        std::cerr << "Error: " << error << std::endl;
      }
        
    std::string res = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responsesstring.size() - responsesstring.find("\r\n\r\n") - 4) +"\r\n"+ responsesstring;
    return res;
}
