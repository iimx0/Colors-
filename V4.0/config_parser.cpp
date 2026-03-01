#include "config_parser.h"
#include "config_validator.h"
#include "config_utils.h"
#include "config_main.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <map>
#include <set>

namespace {
    const std::string USER_KEY = "user";
    const std::string PATH_KEY = "path";
    const std::string SYMBOL_KEY = "symbol$";
    const std::string COMPUTER_KEY = "computer";
    const std::string COLORS_USER_KEY = "colors_user";
    const std::string COLORS_COMPUTER_KEY = "colors_computer";
    const std::string COLORS_PATH_KEY = "colors_path";
    const std::string COLORS_SYMBOL_KEY = "colors_symbol$";
    
    bool is_user_section_start(const std::string& line) {
        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos) return false;
        
        std::string key = line.substr(0, colon_pos);
        std::string value = line.substr(colon_pos + 1);
        
        if (key.empty() || value.empty()) return false;
        
        if (key.find("frame") == 0) return false;
        if (key.find("colors_frame") == 0) return false;
        
        return key != USER_KEY && 
               key != PATH_KEY && 
               key != SYMBOL_KEY &&
               key != COMPUTER_KEY &&
               key != COLORS_USER_KEY &&
               key != COLORS_COMPUTER_KEY &&
               key != COLORS_PATH_KEY &&
               key != COLORS_SYMBOL_KEY;
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
        
        size_t comment_pos = value.find('#', first_non_space);
        if (comment_pos != std::string::npos) {
            value = value.substr(first_non_space, comment_pos - first_non_space);
        } else {
            value = value.substr(first_non_space);
        }
        
        size_t last_non_space = value.find_last_not_of(" \t\r\n");
        if (last_non_space != std::string::npos) {
            value = value.substr(0, last_non_space + 1);
        }
        
        return value;
    }
    
    bool is_frame_key(const std::string& key) {
        if (key == "frame") return true;
        if (key.find("frame") == 0) {
            std::string num_part = key.substr(5);
            if (num_part.empty()) return false;
            for (char c : num_part) {
                if (!isdigit(c)) return false;
            }
            return true;
        }
        return false;
    }
    
    bool is_colors_frame_key(const std::string& key) {
        if (key == "colors_frame") return true;
        if (key.find("colors_frame") == 0) {
            std::string num_part = key.substr(12);
            if (num_part.empty()) return false;
            for (char c : num_part) {
                if (!isdigit(c)) return false;
            }
            return true;
        }
        return false;
    }
    
    int extract_frame_index(const std::string& key) {
        if (key == "frame") return 1;
        if (key == "colors_frame") return 1;
        if (key.find("frame") == 0) {
            return std::atoi(key.substr(5).c_str());
        }
        if (key.find("colors_frame") == 0) {
            return std::atoi(key.substr(12).c_str());
        }
        return 0;
    }
    
    void process_frame_line(UserConfig& config, const std::string& key, const std::string& value) {
        int index = extract_frame_index(key);
        if (index < 1 || index > 9) return;
        
        while (config.frame_sections.size() < index) {
            FrameSection empty_section;
            empty_section.color = -1;
            config.frame_sections.push_back(empty_section);
        }
        
        FrameSection& section = config.frame_sections[index - 1];
        
        if (key.find("colors_frame") == 0) {
            section.color = std::atoi(value.c_str());
        } else {
            section.symbols = split(value, ';');
        }
    }
    
    void process_key_value(UserConfig& config, 
                          const std::string& key, 
                          const std::string& value) {
        if (key == USER_KEY) {
            config.user_tag = value;
        } else if (key == COMPUTER_KEY) {
            config.computer_tag = value;
        } else if (key == PATH_KEY) {
            config.path_tag = value;
        } else if (key == SYMBOL_KEY) {
            config.symbol_tag = value;
        } else if (key == COLORS_USER_KEY) {
            config.colors_user = std::atoi(value.c_str());
        } else if (key == COLORS_COMPUTER_KEY) {
            config.colors_computer = std::atoi(value.c_str());
        } else if (key == COLORS_PATH_KEY) {
            config.colors_path = std::atoi(value.c_str());
        } else if (key == COLORS_SYMBOL_KEY) {
            config.colors_symbol = std::atoi(value.c_str());
        } else if (is_frame_key(key) || is_colors_frame_key(key)) {
            process_frame_line(config, key, value);
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
        
        size_t first_non_space = line.find_first_not_of(" \t");
        if (first_non_space == std::string::npos) continue;
        
        if (line[first_non_space] == '#') {
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
            size_t space_pos = current_user.find_first_of(" \t");
            if (space_pos != std::string::npos) {
                current_user = current_user.substr(0, space_pos);
            }
            
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
    config.colors_user = -1;
    config.colors_computer = -1;
    config.colors_path = -1;
    config.colors_symbol = -1;
    
    if (lines.empty()) {
        return config;
    }
    
    bool has_template = false;
    
    for (const auto& line : lines) {
        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos) {
            continue;
        }
        
        std::string key = line.substr(0, colon_pos);
        size_t space_pos = key.find_first_of(" \t");
        if (space_pos != std::string::npos) {
            key = key.substr(0, space_pos);
        }
        
        std::string value = extract_key_value(line);
        
        if (key == USER_KEY || 
            key == COMPUTER_KEY ||
            key == PATH_KEY || 
            key == SYMBOL_KEY ||
            key == COLORS_USER_KEY ||
            key == COLORS_COMPUTER_KEY ||
            key == COLORS_PATH_KEY ||
            key == COLORS_SYMBOL_KEY ||
            is_frame_key(key) ||
            is_colors_frame_key(key)) {
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
        
        size_t comment_pos = token.find('#');
        if (comment_pos != std::string::npos) {
            token = token.substr(0, comment_pos);
            size_t trim_start = token.find_first_not_of(" \t");
            if (trim_start == std::string::npos) continue;
            size_t trim_end = token.find_last_not_of(" \t");
            token = token.substr(trim_start, trim_end - trim_start + 1);
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
        
        for (size_t i = 0; i < pair.second.frame_sections.size(); ++i) {
            const auto& section = pair.second.frame_sections[i];
            file << "frame" << (i + 1) << ": ";
            for (size_t j = 0; j < section.symbols.size(); ++j) {
                file << section.symbols[j];
                if (j < section.symbols.size() - 1) file << ";";
            }
            file << "\n";
            file << "colors_frame" << (i + 1) << ": " << section.color << "\n";
        }
        
        if (!pair.second.user_tag.empty()) {
            file << "user: " << pair.second.user_tag << "\n";
        }
        
        if (pair.second.colors_user != -1) {
            file << "colors_user: " << pair.second.colors_user << "\n";
        }
        
        if (!pair.second.computer_tag.empty()) {
            file << "computer: " << pair.second.computer_tag << "\n";
        }
        
        if (pair.second.colors_computer != -1) {
            file << "colors_computer: " << pair.second.colors_computer << "\n";
        }
        
        if (!pair.second.path_tag.empty()) {
            file << "path: " << pair.second.path_tag << "\n";
        }
        
        if (pair.second.colors_path != -1) {
            file << "colors_path: " << pair.second.colors_path << "\n";
        }
        
        if (!pair.second.symbol_tag.empty()) {
            file << "symbol$: " << pair.second.symbol_tag << "\n";
        }
        
        if (pair.second.colors_symbol != -1) {
            file << "colors_symbol$: " << pair.second.colors_symbol << "\n";
        }
        
        file << "\n";
    }
    
    file.close();
    return true;
}

bool update_user_config_in_file(const std::string& path, const std::string& username, const UserConfig& config) {
    return save_user_config(path, username, config);
}