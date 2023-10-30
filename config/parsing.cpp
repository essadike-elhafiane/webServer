#include"parsing.hpp"

void error_message(std::string str)
{
    std::cout << str << std::endl;
    exit(1);
}


// void token_elemet(std::vector<std::string> line,std::vector<std::string> &line1 ,const char *c )
// {
//     std::string tmp;
//     int i = 0;
//     while(!line[i].empty())
//     {
//         tmp = line[i];
//         if (std::string::npos != tmp.find(c))
//         {
//             while(!tmp.empty() &&  std::string::npos != tmp.find(c))
//             {
//                 line1.push_back(tmp.substr(0, tmp.find(c)) + ' ');
//                 line1.push_back(c);
//                 tmp = tmp.substr(tmp.find(c) + 1, tmp.length() - tmp.find(c) - 1); 
//             }
//         }
//         if(!tmp.empty() && std::string::npos == tmp.find(c))
//         {
//             line1.push_back(tmp);
//         }
//         i++;
//     }
//     line1.push_back("");
// }



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
}

void listen_port(std::vector<std::string>::iterator &ptr , HTTP_SERVER &m , std::vector<std::string>::iterator l)
{
    int i = 0;
    int j = 0;

    while(ptr != l && i < 3)
    {
        while(i == 1 && (*ptr)[j] != '\0')
        {
            
            if((*ptr)[j] >= '0' && (*ptr)[j] <= '9')
                j++;
            else
            {
                std::cout << "error1 listen" << "\n";
                exit(0);
            }
        }
        if (i == 1)
            m.port.push_back(atoi((*ptr).c_str()));
        
        if(i == 2 && (*ptr) == ";")
        {
            if(m.port[0] > 65536)
            {
                 std::cout << "error2 listen" << "\n";
                exit(0);
            }
            return;
        }
        ptr++;
        i++;
    }
    std::cout << "error3 listen" << "\n";
    exit(0); 
}

void is_alpa_digi(std::string str)
{
    int i = 0;
    while(str[i])
    {
        if ((str[i] >= '0' && str[i] <= '9'  ) || (str[i] >= 'a' && str[i] <= 'z'  ) || (str[i] >= 'A' && str[i] <= 'Z') || str[i] == '-' )
            i++;
        else
        {
            std::cout << str[i];
            error_message("error domaine name");
        }
    }
}

void is_word(std::string str)
{
    int i = 0;
    while(str[i])
    {
        if ((str[i] >= '0' && str[i] <= '9'  ) || (str[i] >= 'a' && str[i] <= 'z'  ) || (str[i] >= 'A' && str[i] <= 'Z') || str[i] == '-' || str[i] == '.')
            i++;
        else
            error_message("error domaine name");
    }
}


int  is_path(std::string str)
{
    int i = 0;
    while(str[i])
    {
        if (str[i] != '{' && str[i] != ';' && str[i] != '}')
            i++;
        else
            return 0;
    }
    return 1;
}

void server_name_parsing(std::vector<std::string>::iterator &ptr , HTTP_SERVER &m , std::vector<std::string>::iterator l)
{
    int i = 0;


    // std::cout << *ptr;
    while(ptr != l && i < 3)
    {
        if(i == 1 && ((*ptr) == "{" || (*ptr) == "}" || (*ptr) == ";"))
            error_message("error2 server_name");
        if (i == 1)
            m.server_name = *ptr;
        if (i == 2 && (*ptr) == ";")
            return;
        ptr++;
        i++;
    }
    std::cout << "error2 server_name" << "\n";
    exit(0); 
}

void root_pars(std::vector<std::string>::iterator &ptr, HTTP_SERVER &m, std::vector<std::string>::iterator l)
{
    int i = 0;

    
    // std::cout << *ptr;
    while(ptr != l && i < 3)
    {
        if(i == 1 && ((*ptr) == ";" || (*ptr) =="{" || (*ptr) == "}"))
            error_message("error2 root");
        if (i == 1)
        {
            m.root = *ptr;
            DIR* directory = opendir(m.root.c_str());
            if (directory == NULL) {
                error_message("faile to open root directory");
            }
        }
        if (i == 2 && (*ptr) == ";")
            return;
        ptr++;
        i++;
    }
    error_message("error root");
    exit(0);
}


// void checkExistDirctory(std::string path)
// {
//     std::vector<std::string> input
//     std::vector<std::string> out; 
//     token_elemet();

// }


void pars_redirection(std::vector<std::string>::iterator &ptr, std::string &m, std::vector<std::string>::iterator l , std::string str)
{
    int i = 0;

    while(ptr != l && i < 3)
    {
        if(i == 1 && ((*ptr) == ";" || (*ptr) =="{" || (*ptr) == "}"))
            error_message(str);
        if (i == 1)
        {
            m = *ptr;
            if (str == "root")
            {
                DIR* directory = opendir(m.c_str());
                if (directory == NULL) 
                {
                    std::cout << "|" << m << "|" << std::endl;
                    error_message("faile to open root directory");
                }
                delete directory->__dd_buf;
                delete directory;
            }
        }
        if (i == 2 && (*ptr) == ";")
            return;
        ptr++;
        i++;
    }
   error_message(str);
}

void index_pars(std::vector<std::string>::iterator &ptr, std::map<std::string,std::string> &m, std::vector<std::string>::iterator l)
{
    int i = 0;


    // std::cout << *ptr;
    while(ptr != l && i < 3)
    {
        if(i == 1 && ((*ptr) == ";" || (*ptr) =="{" || (*ptr) == "}"))
            error_message("error index");
        if (i == 1)
            m["index"] = *ptr; 
        if (i == 2 && (*ptr) == ";")
            return;
        ptr++;
        i++;
    }
    std::cout << "error2 index" <<  "\n";
    exit(0);
}

void parc_cgi(std::vector<std::string>::iterator &ptr, std::map<std::string,std::string> &m, std::vector<std::string>::iterator l)
{
    int i = 0;

    
    while(ptr != l && i < 3)
    {
        if(i == 1 && ((*ptr) == ";" || (*ptr) =="{" || (*ptr) == "}"))
            std::cout << "error2 cgi" <<  "\n";
        if (i == 1)
            m["fastcgi.index"] = *ptr;
        if (i == 2 && (*ptr) != ";" )
            std::cout << "error2 cgi" <<  "\n";
        else if (i == 2)
            return ;
        ptr++;
        i++;
    }
    std::cout << "error2 cgi" <<  "\n";
    exit(0);
}


void size_pars(std::vector<std::string>::iterator &ptr, HTTP_SERVER &m, std::vector<std::string>::iterator l)
{
    int i = 0;
    int j = 0;
    // std::cout << *ptr;
    while(ptr != l && i < 3)
    {
        if(i == 1)
        {
            while((*ptr)[j])
            {
                if (((*ptr)[j] >= '0' && (*ptr)[j] <= '9'))
                    j++;
                else
                    error_message("error client body size");
            }
        }
        if(i == 1)
            m.client_max_body_size = atoi((*ptr).c_str());
        if (i == 2 && (*ptr) == ";")
        {
            return;
        }
        ptr++;
        i++;
    }
    std::cout << "error2 index" <<  "\n";
    exit(0);
}

int is_digit(std::string str)
{
    int i = 0;
    while(str[i])
    {
        if(str[i] >= '0' && str[i] <= '9')
            i++;
        else
            return 0;
    }
    return 1;
}


void assigne_element(HTTP_SERVER &m , std::string str)
{
    std::fstream open(str);
    if (!open.is_open())
    {
        std::cout << str;
        error_message("error open file 2");
    }
   for( std::map<int , std::string>::iterator  ptr = m.error_page.begin() ; ptr != m.error_page.end() ; ptr++)
   {
        if((*ptr).second.empty())
        {
            (*ptr).second = str;        
        }
   }

}

void error_page(std::vector<std::string>::iterator &ptr, HTTP_SERVER &m, std::vector<std::string>::iterator l)
{
    int i = 0;
    // std::cout << *ptr;
    while(ptr != l && i < 3)
    {
        if(i == 1 && !is_digit(*ptr))
            error_message("error in error page");
        if (i == 1)
            m.error_page[atoi((*ptr).c_str())] = "";
        if(i > 1)
        {
            while(ptr != l && is_digit(*ptr))
            {
                m.error_page[atoi((*ptr).c_str())] = "";
                ptr++;
            }
            if(is_path(*ptr) && ptr != l)
            {
                assigne_element(m , (*ptr));
                ptr++;
            }
            else
                error_message("error error in page");
            if((*ptr) == ";" && ptr != l)
                return;
            else
                error_message("error error in page");
        }
        ptr++;
        i++;
    }
    std::cout << "error2 error page" <<  "\n";
    exit(0);
}

void pars_methods(std::vector<std::string>::iterator &ptr, HTTP_SERVER &m, std::vector<std::string>::iterator l)
{
    ptr++;
       
    while(( (*ptr) == "GET" || (*ptr) == "POST" || (*ptr) == "DELETE") && ptr != l)
    {
        for(std::vector<std::string>::iterator itr = m.allow_methods.begin() ; itr !=   m.allow_methods.end() ; itr++)
        {
            if (*itr == *ptr)
                error_message("error duplicate method");
        }
        m.allow_methods.push_back(*ptr);
        ptr++;
    }
    if ((*ptr) == ";" && !m.allow_methods.empty())
    {
        return;
    }
    error_message("error allow methods");
}


void  pars_auto(std::vector<std::string>::iterator &ptr, int &m, std::vector<std::string>::iterator l)
{
     int i = 0;
    
    while(ptr != l && i < 3)
    {
        if (i == 1 && (*ptr) != "on" && (*ptr) != "off")
            std::cout << "error1 auto" <<  "\n";
        if (i == 1)
        {
            if(*ptr == "off")
                m = 0;
            else
                m = 1;
        }
        if (i == 2 && (*ptr) != ";" )
            std::cout << "error2 outo" <<  "\n";
        else if (i == 2 )
            return ;
        ptr++;
        i++;
    }
    std::cout << "error2 outo" <<  "\n";
    exit(0);
}

void location_pars(std::vector<std::string>::iterator &ptr, HTTP_SERVER &m, std::vector<std::string>::iterator l)
{
     int i = 0;

    std::vector<std::string>::iterator l2;
    // std::cout << *ptr;

    l2 = ptr;
    while(l2 != l)
    {
        if(((*l2) == "}"))
            break;
        l2++;
    }
    
    while(ptr != l2)
    {
        // std::cout << (*ptr) <<  "in location" <<std::endl;
        if (i == 1 && ((*ptr) == ";" || (*ptr) =="{" || (*ptr) == "}" || (*ptr)[0] != '/' ))
            error_message("error in location1");
        if (i == 1)
        {
            m.pages.push_back(LOCATION());
            m.pages.back().path = (*ptr);
            
        }
         if (i == 2 && (*ptr) != "{")
            error_message("error in location2");
        else if (i > 2 )
        {
            if((*ptr) == "root")
                pars_redirection(ptr,m.pages.back().root,l2 , "root" );
            else if((*ptr) == "index")
                pars_redirection(ptr,m.pages.back().index,l ,"index");
            else if ((*ptr) == "error_page")
                pars_redirection(ptr,m.error_page_path ,l2 , "error_page_path");
            else if((*ptr) == "allow_methods")
                pars_methods(ptr,m ,l2);
            else if((*ptr) == "return")
                pars_redirection(ptr,m.pages.back().redirection,l2 , "return");
            else if ((*ptr) == "autoindex")
                pars_auto(ptr,m.pages.back().autoindex ,l2);
            else if ( (*ptr) == "cgi_data")
                pars_redirection(ptr,m.pages.back().cgi ,l2 , "cgi_data");
            else 
            {
                std::cout << *ptr;
                error_message("error ");
            }
        }
        ptr++;
        i++;
    }
    // std::cout << "error2 location" <<  "\n";
    // exit(0);
}



void server_pars(std::vector<std::string>::iterator &ptr , Mycfg &obj, HTTP_SERVER &m)
{
    std::vector<std::string>::iterator l;
    l = ptr;
    bracket_part(l , obj);
    ptr++;
    while(ptr != l)
    {
        // std::cout << (*ptr) <<  std::endl;
        if((*ptr) == "port")
            listen_port(ptr , m , l);
        else if((*ptr) == "server_name")
            server_name_parsing(ptr , m , l);
        // else if((*ptr) == "root")
        //     root_pars(ptr,m,l); 
        else if ((*ptr) == "error_page")
            error_page(ptr,m,l);
        else if ((*ptr) == "client_max_body_size")
            size_pars(ptr,m,l);
        else if((*ptr) == "allow_methods")
                pars_methods(ptr,m ,l);
        else if ((*ptr) == "location")
            location_pars(ptr,m,l);
        else if ((*ptr) != ";" && (*ptr) != "{" && (*ptr) != "}")
        {
            std::cout  << "|"<< (*ptr) << "|"<< std::endl;
            error_message("error in valide data");
        }
        ptr++;
    }
}

void token_elemet(std::vector<std::string> line,std::vector<std::string> &line1 ,const char *c )
{
    ;;;;
    std::string tmp;
    int i = 0;
    while(!line[i].empty())
    {
        tmp = line[i];
        if (std::string::npos != tmp.find(c))
        {
            while(!tmp.empty() &&  std::string::npos != tmp.find(c))
            {
                line1.push_back(tmp.substr(0, tmp.find(c)) + ' ');
                line1.push_back(c);
                tmp = tmp.substr(tmp.find(c) + 1, tmp.length() - tmp.find(c) - 1); 
            }
        }
        if(!tmp.empty() && std::string::npos == tmp.find(c))
        {
            line1.push_back(tmp);
        }
        i++;
    }
    line1.push_back("");
}

void clean_element(std::vector<std::string> line, std::vector<std::string> &line1)
{
    for (size_t i = 0; i < line.size(); ++i)
    {
        std::istringstream iss(line[i]);
        std::string token;
        while (iss >> token)
        {
            line1.push_back(token);
        }
    }
}

void chek_line(std::vector<std::string> line)
{
    size_t last;
    size_t first;
    std::string tmp;
    for(std::vector<std::string>::iterator ptr = line.begin() ; ptr != line.end(); ptr++)
    {
        if(std::string::npos != (*ptr).find("location"))
            ptr++;
        else if(*ptr != "{" && *ptr != "}" && std::string::npos == (*ptr).find("server"))
        {
            last = (*ptr).find_last_not_of(" \t");
            first = (*ptr).find_first_not_of(" \t");
            if(std::string::npos == first)
                tmp = "";
            else
                tmp = (*ptr).substr(first , last - first +  1);
            if(!tmp.empty() && tmp[tmp.length() - 1] != ';')
            {
                std:: cout << *ptr << std::endl;
                error_message("error ;");
            }
        }
    }
}

void ValidData(HTTP_SERVER data)
{
    if(data.error_page.empty() || data.port.empty() || data.client_max_body_size == -1)
        error_message("error not enough data");
}


void valid_location(HTTP_SERVER data)
{
    for(std::vector<LOCATION>::iterator ptr = data.pages.begin() ; ptr != data.pages.end() ; ptr++)
    {
        if(ptr->root.empty() || ptr->index.empty())
            error_message("error location not enough data");
    } 
}

void f()
{
    system("leaks webserv");
}

std::vector<HTTP_SERVER>& configFile (int argc , char **argv,std::vector< HTTP_SERVER> &data)
{
    if (argc != 2)
        error_message("error invalide argument");
    char *file = argv[1];
    Mycfg obj;
    std::string tmp;
    std::string tmp1;
    std::string tmp2;
    std::ifstream readcofg(file);
    int s = 0;

    std::string name(file);
    std::cout << name.find(".conf") << "|" << name.length()  << name.length() - 5 << std::endl;
    if(name.length() < 6   || name.find(".conf") != name.length() - 5 )
        error_message("error in file extention");
    std::cout << name.find(".conf") << "|" << name.length() <<std::endl;
    if (!readcofg.is_open())
        error_message("error open cpnfig file");
    while(getline(readcofg,tmp))
    {
        if (std::string::npos != tmp.find("server"))
            s++;
        if(std::string::npos != tmp.find("#"))
            tmp = tmp.substr(0,tmp.find("#"));
        if(!tmp.empty())
        obj.line.push_back(tmp);
    }
        obj.line.push_back("");
    token_elemet(obj.line , obj.line1 ,"{" );
    token_elemet(obj.line1 , obj.line2 ,"}" );
    chek_line(obj.line2);
    token_elemet(obj.line2 , obj.line3 ,";");
    clean_element(obj.line3 , obj.line4);
    // s = 0;
    for(std::vector<std::string>::iterator ptr = obj.line4.begin() ; ptr < obj.line4.end() ;ptr++)
    {
        if(std::string::npos != (*ptr).find("server") && (*ptr).length() == 6)
        {
            data.push_back(HTTP_SERVER()) ;
            server_pars(ptr ,obj , data.back());
        }
        else
        {
            // std::cout << (*ptr) << "|" << std::endl;
            std::cout << "error in server prototype";
            exit(0);
        }
        // s++;
    }
    // exit(0);
    std::vector<HTTP_SERVER>::iterator ptr = data.begin();
    ValidData(*ptr);
    ptr++;
    int flg;
    while(ptr != data.end())
    {
        flg = 0;
        for(std::vector<HTTP_SERVER>::iterator ptr1 = ptr + 1 ; ptr1 != data.end(); ptr1++)
        {
            if((!ptr->server_name.empty() && !ptr1->server_name.empty() &&  ptr->server_name == ptr1->server_name))
                error_message("error ");
        }
        if(ptr != data.begin() && (ptr->client_max_body_size == -1))
            ptr->client_max_body_size = data.begin()->client_max_body_size;
        for(std::vector<LOCATION>::iterator ptr2 = ptr->pages.begin() ; ptr2 != ptr->pages.end(); ptr2++ )
        {
            if(ptr2->path == "/")
                flg = 1;
            if(ptr->root.empty())
                error_message("error not root /");     
        }
        if(flg == 0)
            error_message("error not root /");
        ptr++;
    }
    //     std::cout << *ptr << std::endl;
   
   
    return data;
    // std::vector< HTTP_SERVER> ata  = data;
    // std::cout << "\n\n\n\n";
    // for(std::vector<HTTP_SERVER>::iterator ptr =ata.begin() ; ptr != ata.end(); ptr++ )
    //     std::cout << *ptr << std::endl;
    // return 0;
}
