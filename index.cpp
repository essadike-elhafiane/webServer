#include <iostream>
#include <fstream>
#include <dirent.h>



int indexGenerator(std::string url , std::string name )
{
    std::string path = "/Users/mserrouk/Desktop/webserver";
    std::string html = name + ".html";

    std::ofstream write(html);
    if (! write.is_open())
    {
        return 0;
       
    }
    std::cout << "jj";
   write << "<!DOCTYPE html>\n";
   write << "<html lang=\"en\">\n";
   write << "<head>\n";
   write << "<meta charset=\"UTF-8\">\n";
   write << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
   write << "<title>" << path << "</title>\n";
    write << "<h1>" << "index " << path << "<h1>\n";
    write << "<hr>\n<ul>\n";
    DIR *directory = opendir(path.c_str());
    if(directory == NULL)
    {
        write << "<h1>" << "error open path" << path << "<h1>\n";
        return 0;
    }
        delete directory->__dd_buf;
        delete directory;
    struct dirent* file;
    while ((file = readdir(directory)) != NULL)
    {
        write << "<li><div style=\" padding: 20px; color: rgb(224, 190, 141); margin: 5px;\"><a href=\"" << file->d_name << "\">" << file->d_name << "</a></div></li>\n" ; 
    }
    return 1;
}