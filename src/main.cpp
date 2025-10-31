#include "Application.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    try {
        // Seed random number generator
        srand(static_cast<unsigned>(time(nullptr)));
        
        Physica::Application app;
        app.run();
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
