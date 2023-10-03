#include <iostream>
#include <string>


int solve(int a)
{
    int result = 0;

    if (a < 0)
        return 0;
    if (a == 0)
        return 1;
    for (size_t i = 1; i < 7; i++)
    {
        result += solve(a - i);
        if (result < 0)
            break;
    }
    return result;
}

int main()
{
    int a;
    std::cin >> a;

    std::cout << solve(a);
}