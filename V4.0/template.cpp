#include <iostream>
#include <string>
#include <vector>
#include "config_main.h"
#include "template.h"
#include "prompt.h"
#include "colors.h"

std::string apply_template(const UserConfig& config, const std::string& username, const std::string& current_dir, const std::string& symbol) {
    std::string result = config.template_string;
    std::string computer_name = get_computer_name();
    
    if (!config.user_tag.empty()) {
        size_t pos = 0;
        while ((pos = result.find(config.user_tag, pos)) != std::string::npos) {
            result.replace(pos, config.user_tag.length(), username);
            pos += username.length();
        }
    }
    
    if (!config.computer_tag.empty()) {
        size_t pos = 0;
        while ((pos = result.find(config.computer_tag, pos)) != std::string::npos) {
            result.replace(pos, config.computer_tag.length(), computer_name);
            pos += computer_name.length();
        }
    }
    
    if (!config.path_tag.empty()) {
        size_t pos = 0;
        while ((pos = result.find(config.path_tag, pos)) != std::string::npos) {
            result.replace(pos, config.path_tag.length(), current_dir);
            pos += current_dir.length();
        }
    }
    
    if (!config.symbol_tag.empty()) {
        size_t pos = 0;
        while ((pos = result.find(config.symbol_tag, pos)) != std::string::npos) {
            result.replace(pos, config.symbol_tag.length(), symbol);
            pos += symbol.length();
        }
    }
    
    size_t pos = 0;
    while ((pos = result.find("\\n", pos)) != std::string::npos) {
        result.replace(pos, 2, "\n");
        pos += 1;
    }
    
    result = apply_colors_to_prompt(result, config, username, computer_name, current_dir, symbol);
    
    return result;
}