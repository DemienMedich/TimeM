#include <iostream>
#include <ctime>
#include <cstdlib>
#include "../include/UserInterface.h"

int main() {
    // Set random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Create and run the user interface
    UserInterface ui;
    ui.run();
    
    return 0;
}