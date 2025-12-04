#include <iostream>
#include <string>
#include <vector>
#include "config.h"
#include "template.h"

std::string apply_template(const UserConfig& config, const std::string& username, const std::string& current_dir, const std::string& symbol) {
    std::string result = config.template_string;
    
    size_t pos = 0;
    while ((pos = result.find(config.user_tag, pos)) != std::string::npos) {
        result.replace(pos, config.user_tag.length(), username);
        pos += username.length();
    }
    
    pos = 0;
    while ((pos = result.find(config.path_tag, pos)) != std::string::npos) {
        result.replace(pos, config.path_tag.length(), current_dir);
        pos += current_dir.length();
    }
    
    pos = 0;
    while ((pos = result.find(config.symbol_tag, pos)) != std::string::npos) {
        result.replace(pos, config.symbol_tag.length(), symbol);
        pos += symbol.length();
    }
    
    if (config.frame_symbols.size() >= 5) {
        pos = 0;
        while ((pos = result.find("─", pos)) != std::string::npos) {
            result.replace(pos, 3, config.frame_symbols[0]);
            pos += config.frame_symbols[0].length();
        }
        
        pos = 0;
        while ((pos = result.find("┌", pos)) != std::string::npos) {
            result.replace(pos, 3, config.frame_symbols[1]);
            pos += config.frame_symbols[1].length();
        }
        
        pos = 0;
        while ((pos = result.find("└", pos)) != std::string::npos) {
            result.replace(pos, 3, config.frame_symbols[2]);
            pos += config.frame_symbols[2].length();
        }
        
        pos = 0;
        while ((pos = result.find("[", pos)) != std::string::npos) {
            result.replace(pos, 1, config.frame_symbols[3]);
            pos += config.frame_symbols[3].length();
        }
        
        pos = 0;
        while ((pos = result.find("]", pos)) != std::string::npos) {
            result.replace(pos, 1, config.frame_symbols[4]);
            pos += config.frame_symbols[4].length();
        }
    }
    
    pos = 0;
    while ((pos = result.find("<", pos)) != std::string::npos) {
        size_t end_pos = result.find(">", pos);
        if (end_pos != std::string::npos) {
            std::string content = result.substr(pos + 1, end_pos - pos - 1);
            result.replace(pos, end_pos - pos + 1, content);
            pos += content.length();
        } else {
            break;
        }
    }
    
    pos = 0;
    while ((pos = result.find("\\n", pos)) != std::string::npos) {
        result.replace(pos, 2, "\n");
        pos += 1;
    }
    
    return result;
}