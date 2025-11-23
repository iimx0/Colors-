#include <iostream>
#include <string>
#include "config.h"
#include "prompt.h"
#include "template.h"

int main() {
    std::string username = get_username();
    
    if (username.empty()) {
        return 1;
    }
    
    UserConfig config = get_user_config(username);
    
    if (config.template_string.empty()) {
        if (!add_new_user(username)) {
            return 1;
        }
        config = get_user_config(username);
        
        if (config.template_string.empty()) {
            return 1;
        }
    }
    
    std::string current_dir = get_current_dir();
    std::string symbol = (username == "root") ? "#" : "$";
    
    std::string final_prompt = apply_template(config, username, current_dir, symbol);
    
    std::cout << final_prompt;
    
    return 0;
}
