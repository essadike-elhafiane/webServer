#include <iostream>
#include <string>
#include <map>

// int solve(int a)
// {
//     int result = 0;

//     if (a < 0)
//         return 0;
//     if (a == 0)
//         return 1;
//     for (size_t i = 1; i < 7; i++)
//     {
//         result += solve(a - i);
//         if (result < 0)
//             break;
//     }
//     return result;
// }
void 
int main()
{
    atexit(f);
    std::map<std::string, int> m;

    m["ahmed"] = 1;
    m["mohamed"] = 2;
    
    m.erase("ahmed");
    std::cout << m.find("ahmed")->second << std::endl;
    // int a;
    // std::cin >> a;

    // std::cout << solve(a);
}