#include"CGI.hpp"
#include <fstream>




std::string mainCGI(std::string urlCgi, int clientSocket, Client &dataClient) {


    HttpRequest request;
  
    std::cout<<urlCgi<<std::endl;
    (void) clientSocket;
  
    std::string spath ="/Users/eelhafia/Desktop/webServer/CGI/hello_script.php";
    std::string sfile = "/Users/eelhafia/Desktop/webServer/CGI/hello_script.php";
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
   

    //std::string path = "/Users/eelhafia/Desktop/fixweb/CGIfix/test.py";
    std::string path ="/Users/eelhafia/Desktop/webServer/CGI/hello_script.php";
   
    std::string file = "/Users/eelhafia/Desktop/webServer/CGI/hello_script.php";
    std::string responsesstring;
     try {

        std::string scriptType = "php";
        std::cout << "sdgsdgsdgsdgsdgs\n";

        cgiSettler.executionCGI();
        cgiSettler.dataClient = dataClient;
        wait(0);
        std::cout << "sdgsdgsdgsdgsdgs------------------\n";
        std::string postDate = "my data";
        int readEnd = cgiSettler.getReadEnd();

        char c[1001];
        memset(c, 0, 1000);
        while(read(readEnd ,c,1000))
        {
          responsesstring += c;
          memset(c, 0, 1000);
           std::cout << "????????????\n";
        }
        //std::cout<< responsesstring << std::endl;
        cgiSettler.close_pipes();
     } catch (const char* error) {
        std::cerr << "Error: " << error << std::endl;
      }
        
    std::string res = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responsesstring.size() - responsesstring.find("\r\n\r\n") - 4) +"\r\n"+ responsesstring;
    return res;
}
