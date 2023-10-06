#include"parsing.hpp"


void bracket_part(std::vector<std::string>::iterator &ptr , Mycfg &obj)
{
    int o = 0;
    int c = 0;
    int i = 0;
    ptr++;
    while(ptr < obj.line4.end())
    {

        if(std::string::npos != (*ptr).find("{"))
            o++;
        if(std::string::npos != (*ptr).find("}"))
            c++;
        if(c > o || c  + 3 <= o || (i == 0 && std::string::npos == (*ptr).find("{")))
        {
            std::cout << "error in bracket" << std::endl;
            exit(0);
        }
        if (c == o )
        {
            return ;
        }
        ptr++;
        i++;
    }
    if (c == 0 )
    {
        std::cout << "error no server bracket" << std::endl;
            exit(0);
    }
    // exit(0);
}


void listen_parsing(std::vector<std::string>::iterator &ptr , Mycfg &obj , std::vector<std::string>::iterator l)
{
    int i = 0;
    int j = 0;
    while(ptr != l && i < 3)
    {
        if(i == 1)
        {
            while((*ptr)[j] != '\0')
            {
                if((*ptr)[j] >= 0 && (*ptr)[j] <= 9)
                j++;
                else
                {
                    std::cout << "error1" << "\n";
                    exit(0);
                }
            }
        }
        if( i == 2)

        ptr++;
        i++;
    }
    std::cout << "error1" << "\n";
    exit(0);


    
}

void server_pars(std::vector<std::string>::iterator &ptr , Mycfg &obj)
{
    std::vector<std::string>::iterator l;
    l = ptr;
    bracket_part(l , obj);
    while(ptr != l)
    {
        if((*ptr) == "listen")
            listen_parsing(ptr , obj , l);


        ptr++;
    }
    // ptr = l;
    
    // exit(0);

}

void token_elemet(std::vector<std::string> line,std::vector<std::string> &line1 ,const char *c , int j)
{
    std::string tmp;
    int i = 0;
    while(!line[i].empty())
    {
        tmp = line[i];
        // if (j == 1)
        //     std::cout <<i << std::endl;
        if (std::string::npos != tmp.find(c))
        {
            while(!tmp.empty() &&  std::string::npos != tmp.find(c))
            {
                // std::cout << tmp.substr(0, tmp.find(c)) + ' ' <<  std::endl;
                // std::cout << c <<  std::endl;
                line1.push_back(tmp.substr(0, tmp.find(c)) + ' ');
                line1.push_back(c);
                tmp = tmp.substr(tmp.find(c) + 1, tmp.length() - tmp.find(c) - 1); 
            }
        }
        if(!tmp.empty() && std::string::npos == tmp.find(c))
        {
            // std::cout << tmp <<  std::endl;
            line1.push_back(tmp);
        }
        i++;
    }
    line1.push_back("");
    // std::cout << "<<<<" << std::endl;
    // i = 0;
    // while(!line1[i].empty())
    // {
        
    //     std::cout <<i << line1[i] << "|"<<  std::endl;
    //     i++;
    // }
    
}

void clean_element(std::vector<std::string> line,std::vector<std::string> &line1)
{
    char *token;
    int i = 0;
    while(!line[i].empty())
    {
        token = std::strtok( (char*)line[i].c_str(), " \t\n\r");
        while (token != nullptr)
        {
            line1.push_back(token);
            token = std::strtok( nullptr, " \t\n\r");
        }
        i++;      
    }
}


int main (int argc , char **argv)
{
    char *file = argv[1];
    Mycfg obj;
    char *token;
    char *str;
    std::string tmp;
    std::string tmp1;
    std::string tmp2;
    std::ifstream readcofg(file);

    if (!readcofg.is_open())
        return (std::cout << "error in file\n" ,readcofg.close() , 0);
    while(getline(readcofg,tmp))
    {
        if(std::string::npos != tmp.find("#"))
            tmp = tmp.substr(0,tmp.find("#"));
        if(!tmp.empty())
        obj.line.push_back(tmp);
    }
        obj.line.push_back("");
    token_elemet(obj.line , obj.line1 ,"{" , 0);
    token_elemet(obj.line1 , obj.line2 ,"}" , 1); 
    token_elemet(obj.line2 , obj.line3 ,";", 2);
    clean_element(obj.line3 , obj.line4);
    for(std::vector<std::string>::iterator ptr = obj.line4.begin() ; ptr < obj.line4.end() ;ptr++)
    {
        if(std::string::npos != (*ptr).find("server") && (*ptr).length() == 6)
            server_pars(ptr ,obj);
        else
        {
            // std::cout << (*ptr) << "|" << std::endl;
            std::cout << "error in server prototype";
            exit(0);
        }
    }
    return 0;
}
