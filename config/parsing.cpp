#include"parsing.hpp"

void error_message(std::string str)
{
    std::cout << str << std::endl;
    exit(1);
}

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

void listen_parsing(std::vector<std::string>::iterator &ptr , std::map<std::string,std::string> &m , std::vector<std::string>::iterator l)
{
    int i = 0;
    int j = 0;

    while(ptr != l && i < 3)
    {
        while(i == 1 && (*ptr)[j] != '\0')
        {
            m["listen"] = *ptr;
            if((*ptr)[j] >= '0' && (*ptr)[j] <= '9')
                j++;
            else
            {
                std::cout << "error1 listen" << "\n";
                exit(0);
            }
        }
        if(i == 2 && (*ptr) == ";")
        {
            if(atoi(m["listen"].c_str()) > 65536)
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

void server_name_parsing(std::vector<std::string>::iterator &ptr , std::map<std::string,std::string> &m , std::vector<std::string>::iterator l)
{
    int i = 0;
    int j = 0;

    // std::cout << *ptr;
    while(ptr != l && i < 3)
    {
        if(i == 1 && ((*ptr) == "{" || (*ptr) == "}" || (*ptr) == ";"))
            error_message("error2 server_name");
        if (i == 1)
            m["server_name"] = *ptr;
        if (i == 2 && (*ptr) == ";")
        {
            return;
        }
        ptr++;
        i++;
    }
    std::cout << "error2 server_name" << "\n";
    exit(0); 
}

void root_pars(std::vector<std::string>::iterator &ptr, std::map<std::string,std::string> &m, std::vector<std::string>::iterator l)
{
    int i = 0;
    int j = 0;
    
    // std::cout << *ptr;
    while(ptr != l && i < 3)
    {
        if(i == 1 && ((*ptr) == ";" || (*ptr) =="{" || (*ptr) == "}"))
        {
            std::cout << "error2 root" <<  "\n";
        }
        if (i == 1)
            m["root"] = *ptr;
        if (i == 2 && (*ptr) == ";")
        {
            return;
        }
        ptr++;
        i++;
    }
    std::cout << "error2 root" <<  "\n";
    exit(0);
}



void pars_redirection(std::vector<std::string>::iterator &ptr, std::map<std::string,std::string> &m, std::vector<std::string>::iterator l , std::string str)
{
    int i = 0;
    int j = 0;
    
    while(ptr != l && i < 3)
    {
        if(i == 1 && ((*ptr) == ";" || (*ptr) =="{" || (*ptr) == "}"))
            std::cout << "error2 " << str <<  "\n";
        if (i == 1)
            m[str] = *ptr;
        if (i == 2 && (*ptr) == ";")
            return;
        ptr++;
        i++;
    }
    std::cout << "error2" << str <<  "\n";
    exit(0);
}

void index_pars(std::vector<std::string>::iterator &ptr, std::map<std::string,std::string> &m, std::vector<std::string>::iterator l)
{
    int i = 0;
    int j = 0;

    // std::cout << *ptr;
    while(ptr != l && i < 3)
    {
        if(i == 1 && ((*ptr) == ";" || (*ptr) =="{" || (*ptr) == "}"))
        {
            std::cout << "error2 index" <<  "\n";
        }
        if (i == 1)
            m["index"] = *ptr; 
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

void parc_cgi(std::vector<std::string>::iterator &ptr, std::map<std::string,std::string> &m, std::vector<std::string>::iterator l)
{
    int i = 0;
    int j = 0;
    
    while(ptr != l && i < 3)
    {
        if(i == 1 && ((*ptr) == ";" || (*ptr) =="{" || (*ptr) == "}"))
            std::cout << "error2 redirection" <<  "\n";
        if (i == 1)
            m["fastcgi.index"] = *ptr;
        if (i == 2 && (*ptr) != ";" )
            std::cout << "error2 redirection" <<  "\n";
        else if (i == 2)
            return ;
        ptr++;
        i++;
    }
    std::cout << "error2 redirection" <<  "\n";
    exit(0);
}


void size_pars(std::vector<std::string>::iterator &ptr, std::map<std::string,std::string> &m, std::vector<std::string>::iterator l)
{
    int i = 0;
    int j = 0;
    // std::cout << *ptr;
    while(ptr != l && i < 3)
    {
        if(i == 1)
        {
            m["client_max_body_size"] = *ptr;
            while((*ptr)[j])
            {
                if (((*ptr)[j] >= '0' && (*ptr)[j] <= '9') || (j == (*ptr).length() - 1 && (*ptr)[j] == 'M'))
                    j++;
                else
                    error_message("error client body size");
            }
        }
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

void error_page(std::vector<std::string>::iterator &ptr, std::map<std::string,std::string> &m, std::vector<std::string>::iterator l)
{
    int i = 0;
    // std::cout << *ptr;
    while(ptr != l && i < 3)
    {
        if(i == 1 && !is_digit(*ptr))
            error_message("error in error page");
        if (i == 1)
            m["error_page"] =   m["error_page"] + " " + *ptr;
        if(i > 1)
        {
            while(ptr != l && is_digit(*ptr))
            {
                m["error_page"] = m["error_page"] + " " + *ptr;
                ptr++;
            }
            if(is_path(*ptr) && ptr != l)
            {
                m["error_page"] = m["error_page"] + " " + *ptr;
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

void pars_limit(std::vector<std::string>::iterator &ptr, std::map<std::string,std::string> &m, std::vector<std::string>::iterator l)
{
    int i = 0;
    int j = 0;
    
    while(ptr != l )
    {
        if(i == 1)
            m["limit_execpet"] = " ";
        while(i >= 1 && ( (*ptr) == "GET" || (*ptr) == "POST" || (*ptr) == "DELETE"))
        {
            m["limit_execpet"] = m["limit_execpet"] + " " + *ptr;
            ptr++;
        }
        if ((*ptr) == ";" && m["limit_execpet"] != " ")
            return;
        else
            std::cout << "error2 limit" <<  "\n";
        ptr++;
        i++;
    }
    std::cout << "error2 limit" <<  "\n";
    exit(0);
}


void  pars_auto(std::vector<std::string>::iterator &ptr, std::map<std::string,std::string> &m, std::vector<std::string>::iterator l)
{
     int i = 0;
    int j = 0;
    
    while(ptr != l && i < 3)
    {
        if (i == 1 && (*ptr) != "on" && (*ptr) != "of")
            std::cout << "error2 redirection" <<  "\n";
        if (i == 1)
        m["auto"] = *ptr;
        if (i == 2 && (*ptr) != ";" )
            std::cout << "error2 redirection" <<  "\n";
        else if (i == 2 )
            return ;
        ptr++;
        i++;
    }
    std::cout << "error2 redirection" <<  "\n";
    exit(0);
}



void location_pars(std::vector<std::string>::iterator &ptr, std::map<std::string,std::string> &m, std::vector<std::string>::iterator l)
{
     int i = 0;
    int j = 0;
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
        if (i == 1 && ((*ptr) == ";" || (*ptr) =="{" || (*ptr) == "}"))
            error_message("error in location1");
         if (i == 2 && (*ptr) != "{")
            error_message("error in location2");
        else if (i > 2 )
        {
            if((*ptr) == "root")
                pars_redirection(ptr,m,l2 , "root");
            else if ((*ptr) == "error_page")
                pars_redirection(ptr,m,l2 , "error_page_path");
            else if((*ptr) == "limit_execpet")
                pars_limit(ptr,m,l2);
            else if((*ptr) == "return")
                pars_redirection(ptr,m,l2 , "return");
            else if ((*ptr) == "autoindex")
                pars_auto(ptr,m,l2);
            else if ( (*ptr) == "fastcgi.index")
                pars_redirection(ptr,m,l2 , "fastcgi.index");
            // else 
            //     error_message("jj");
        }
        ptr++;
        i++;
    }
    // std::cout << "error2 location" <<  "\n";
    // exit(0);
}

void server_pars(std::vector<std::string>::iterator &ptr , Mycfg &obj, std::map<std::string,std::string> &m)
{
    std::vector<std::string>::iterator l;
    l = ptr;
    bracket_part(l , obj);
    m["server"] = "server";
    m["error_page"] = " ";
    while(ptr != l)
    {
        // std::cout << (*ptr) <<  std::endl;
        if((*ptr) == "listen")
            listen_parsing(ptr , m , l);
        else if((*ptr) == "server_name")
            server_name_parsing(ptr , m , l);
        else if((*ptr) == "root")
            root_pars(ptr,m,l);
        else if((*ptr) == "index")
            pars_redirection(ptr,m,l ,"index");
        else if ((*ptr) == "error_page")
            error_page(ptr,m,l);
        else if ((*ptr) == "client_max_body_size")
            size_pars(ptr,m,l);
        else if ((*ptr) == "location")
            location_pars(ptr,m,l);
        // std::cout << *ptr << std::endl;
        ptr++;
    }
}

void token_elemet(std::vector<std::string> line,std::vector<std::string> &line1 ,const char *c , int j)
{
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
    int s;

    if (!readcofg.is_open())
        return (std::cout << "error in file\n" ,readcofg.close() , 0);
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
    token_elemet(obj.line , obj.line1 ,"{" , 0);
    token_elemet(obj.line1 , obj.line2 ,"}" , 1);
    token_elemet(obj.line2 , obj.line3 ,";", 2);
    clean_element(obj.line3 , obj.line4);
    std::map<std::string , std::string> map[s];
    s = 0;
    for(std::vector<std::string>::iterator ptr = obj.line4.begin() ; ptr < obj.line4.end() ;ptr++)
    {
        if(std::string::npos != (*ptr).find("server") && (*ptr).length() == 6)
            server_pars(ptr ,obj , map[s]);
        else
        {
            // std::cout << (*ptr) << "|" << std::endl;
            std::cout << "error in server prototype";
            exit(0);
        }
        s++;
    }

for(int i = 0 ; i < s; i++)
{
    for(std::map<std::string , std::string>::iterator ptr =map[i].begin() ; ptr !=map[i].end(); ptr++ )
        std::cout << ptr->first <<  "  "  << ptr->second << std::endl;
}

    return 0;
}
