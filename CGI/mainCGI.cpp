#include"CGI.hpp"
#include <fstream>




std::string mainCGI(std::string urlCgi, int clientSocket, Client &dataClient) {


    HttpRequest request;
  
    std::cout<<urlCgi<<std::endl;
    (void) clientSocket;
  
    // std::string spath ="/Users/edraidry/Desktop/webserver/CGI/hello_script.php";
    // std::string sfile = "/Users/edraidry/Desktop/webserver/CGI/hello_script.php";
    std::string spath ="/Users/edraidry/Desktop/webserver/CGI/hello_cgi.py" ;
    std::string sfile ="/Users/edraidry/Desktop/webserver/CGI/hello_cgi.py" ;
    std::string sscriptType = "py";
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
   

    std::string path = "/Users/edraidry/Desktop/fixweb/CGIfix/test.py";
    //std::string path ="/Users/edraidry/Desktop/webserver/CGI/hello_script.php";
   
    //std::string file = "/Users/edraidry/Desktop/webserver/CGI/hello_script.php";
    std::string responsesstring;
     try {

        std::string scriptType = "php";

        cgiSettler.executionCGI();
        cgiSettler.dataClient = dataClient;
        wait(0);
       
        std::string postDate = "my data";
        int readEnd = cgiSettler.getReadEnd();

        char c;
        while(read(readEnd ,&c,1))
        {
          responsesstring += c;
           
        }
        if(read(readEnd,&c,1) == 0)
        {
          throw 500;
          return " ";
        }
        std::cout << responsesstring << std::endl;
    
     } catch (int error) {
        std::cerr << "Error: " << error << std::endl;

        dataClient.error = error;
        return " ";
      }
        std::cout<< responsesstring.size() << "|" << responsesstring.find("\r\n\r\n") << std::endl;
    std::string res = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responsesstring.size() - responsesstring.find("\r\n\r\n") - 4) +"\r\n"+ responsesstring;
    return res;
}
