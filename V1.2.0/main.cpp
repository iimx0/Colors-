#include "app_logic.h"
#include "error.h"
#include "help.h"
#include <iostream>
#include <cstdlib>
#include <string>

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "help" || arg == "h") {
            print_help();
            return EXIT_SUCCESS;
        }
    }
    
    try {
        std::string prompt;
        ErrorCode result = generate_prompt(prompt);
        
        if (result != SUCCESS) {
            print_error(result);
            return EXIT_FAILURE;
        }
        
        print_prompt(prompt);
        return EXIT_SUCCESS;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred" << std::endl;
        return EXIT_FAILURE;
    }
}