#include "config_parser.h"
#include "config_validator.h"
#include "config_utils.h"
#include "config_main.h"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace {
    const std::string FRAME_KEY = "frame";
    const std::string USER_KEY = "user";
    const std::string PATH_KEY = "path";
    const std::string SYMBOL_KEY = "symbol$";
    const std::string COMPUTER_KEY = "computer";
    
    bool is_user_section_start(const std::string& line) {
        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos) return false;
        
        std::string key = line.substr(0, colon_pos);
        std::string value = line.substr(colon_pos + 1);
        
        if (key.empty() || value.empty()) return false;
        
        return key != FRAME_KEY && 
               key != USER_KEY && 
               key != PATH_KEY && 
               key != SYMBOL_KEY &&
               key != COMPUTER_KEY;
    }
    
    std::string extract_template_value(const std::string& line) {
        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos) return "";
        
        std::string value = line.substr(colon_pos + 1);
        
        size_t first_non_space = value.find_first_not_of(' ');
        if (first_non_space == std::string::npos) return "";
        
        value = value.substr(first_non_space);
        
        if (value.front() == '"' && value.back() == '"') {
            value = value.substr(1, value.length() - 2);
        }
        
        return value;
    }
    
    std::string extract_key_value(const std::string& line) {
        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos) return "";
        
        std::string value = line.substr(colon_pos + 1);
        
        size_t first_non_space = value.find_first_not_of(' ');
        if (first_non_space == std::string::npos) return "";
        
        return value.substr(first_non_space);
    }
    
    void process_key_value(UserConfig& config, 
                          const std::string& key, 
                          const std::string& value) {
        if (key == FRAME_KEY) {
            config.frame_symbols = split(value, ';');
        } else if (key == USER_KEY) {
            config.user_tag = value;
        } else if (key == COMPUTER_KEY) {
            config.computer_tag = value;
        } else if (key == PATH_KEY) {
            config.path_tag = value;
        } else if (key == SYMBOL_KEY) {
            config.symbol_tag = value;
        }
    }
}

std::map<std::string, UserConfig> parse_config_file(const std::string& path) {
    std::map<std::string, UserConfig> result;
    
    if (!file_exists(path)) {
        return result;
    }
    
    std::ifstream file(path);
    if (!file.is_open()) {
        return result;
    }
    
    std::string line;
    std::string current_user;
    std::vector<std::string> current_lines;
    std::vector<std::string> processed_users;
    
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        
        if (line[0] == '#') {
            continue;
        }
        
        if (is_user_section_start(line)) {
            if (!current_user.empty()) {
                if (std::find(processed_users.begin(), 
                             processed_users.end(), 
                             current_user) == processed_users.end()) {
                    processed_users.push_back(current_user);
                    UserConfig config = parse_user_section(current_lines);
                    if (!config.template_string.empty()) {
                        result[current_user] = config;
                    }
                }
                current_lines.clear();
            }
            
            size_t colon_pos = line.find(':');
            current_user = line.substr(0, colon_pos);
            std::string template_value = extract_template_value(line);
            
            if (!template_value.empty()) {
                current_lines.push_back(line);
            } else {
                current_user.clear();
            }
        } else if (!current_user.empty()) {
            size_t colon_pos = line.find(':');
            if (colon_pos != std::string::npos) {
                current_lines.push_back(line);
            }
        }
    }
    
    if (!current_user.empty()) {
        if (std::find(processed_users.begin(), 
                     processed_users.end(), 
                     current_user) == processed_users.end()) {
            UserConfig config = parse_user_section(current_lines);
            if (!config.template_string.empty()) {
                result[current_user] = config;
            }
        }
    }
    
    file.close();
    return result;
}

UserConfig parse_user_section(const std::vector<std::string>& lines) {
    UserConfig config;
    
    if (lines.empty()) {
        return config;
    }
    
    bool has_template = false;
    
    for (const auto& line : lines) {
        if (line.find(':') == std::string::npos) {
            continue;
        }
        
        size_t colon_pos = line.find(':');
        std::string key = line.substr(0, colon_pos);
        
        if (key == FRAME_KEY || 
            key == USER_KEY || 
            key == COMPUTER_KEY ||
            key == PATH_KEY || 
            key == SYMBOL_KEY) {
            std::string value = extract_key_value(line);
            process_key_value(config, key, value);
        } else {
            config.template_string = extract_template_value(line);
            has_template = true;
        }
    }
    
    if (!has_template) {
        config.template_string.clear();
    }
    
    return config;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        size_t start = token.find_first_not_of(" \t");
        if (start == std::string::npos) {
            continue;
        }
        
        size_t end = token.find_last_not_of(" \t");
        token = token.substr(start, end - start + 1);
        
        if (token.empty()) {
            continue;
        }
        
        size_t space_pos = token.find(' ');
        if (space_pos != std::string::npos) {
            token = token.substr(0, space_pos);
            
            start = token.find_first_not_of(" \t");
            if (start == std::string::npos) {
                continue;
            }
            end = token.find_last_not_of(" \t");
            if (end == std::string::npos) {
                continue;
            }
            token = token.substr(start, end - start + 1);
        }
        
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

bool save_user_config(const std::string& path, const std::string& username, const UserConfig& config) {
    std::map<std::string, UserConfig> all_configs = parse_config_file(path);
    
    all_configs[username] = config;
    
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    
    file << "# Prompt Configuration\n";
    file << "# Auto-generated by colorc\n\n";
    
    for (const auto& pair : all_configs) {
        file << pair.first << ": " << pair.second.template_string << "\n";
        
        if (!pair.second.frame_symbols.empty()) {
            file << "frame: ";
            for (size_t i = 0; i < pair.second.frame_symbols.size(); ++i) {
                file << pair.second.frame_symbols[i];
                if (i < pair.second.frame_symbols.size() - 1) file << ";";
            }
            file << "\n";
        }
        
        if (!pair.second.user_tag.empty()) {
            file << "user: " << pair.second.user_tag << "\n";
        }
        
        if (!pair.second.computer_tag.empty()) {
            file << "computer: " << pair.second.computer_tag << "\n";
        }
        
        if (!pair.second.path_tag.empty()) {
            file << "path: " << pair.second.path_tag << "\n";
        }
        
        if (!pair.second.symbol_tag.empty()) {
            file << "symbol$: " << pair.second.symbol_tag << "\n";
        }
        
        file << "\n";
    }
    
    file.close();
    return true;
}

bool update_user_config_in_file(const std::string& path, const std::string& username, const UserConfig& config) {
    return save_user_config(path, username, config);
}