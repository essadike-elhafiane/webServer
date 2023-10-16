#include <iostream>
#include <unistd.h>

int main() {
    int loadingBarWidth = 40;
    // std::string loadingAnimation = "|/-\\";
    
    // for (int i = 0; i < 100; ++i) {
    //     std::cout << "Loading: [";
        
    //     int progress = (i * loadingBarWidth) / 100;
    //     int animationIndex = i % loadingAnimation.size();
        
    //     for (int j = 0; j < loadingBarWidth; ++j) {
    //         if (j < progress) {
    //             std::cout << "=";
    //         } else if (j == progress) {
    //             std::cout << loadingAnimation[animationIndex];
    //         } else {
    //             std::cout << " ";
    //         }
    //     }
        
    //     std::cout << "] " << i << "%";
    //     std::cout.flush();
        
    //     usleep(100000); // Sleep for 100 milliseconds
    //     std::cout << "\r"; // Move the cursor to the beginning of the line
    // }
    for (size_t i = 0; i < 1; i++)
    {
        for (size_t i = 0; i < 10; i++)
        {
            std::cout << "h" ;
        }
        
        std::cout << "\r";
        std::cout << "aaaaaaaaaaahhhhhhhhhhaaaaaaaaaaa";
        std::cout << "\r";
        std::cout << "ss";
    }
    
    std::cout << std::endl;
    
    return 0;
}
