#include <iostream>
#include <string>
#include <cctype>
#include "config.h"
#include "prompt.h"
#include "template.h"
#include "error.h"

bool is_valid_tag(const std::string& tag) {
    if (tag.length() < 3) return false;
    if (tag.front() != '<' || tag.back() != '>') return false;
    
    for (size_t i = 1; i < tag.length() - 1; i++) {
        char c = tag[i];
        if (!std::isalnum(c) && c != '_' && c != '$') return false;
    }
    return true;
}

bool has_valid_escape(const std::string& str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '\\') {
            if (i + 1 >= str.length()) return false;
            if (str[i + 1] != 'n') return false;
            i++;
        }
    }
    return true;
}

bool starts_with_valid_frame(const std::string& str) {
    if (str.empty()) return false;
    
    if (is_unicode_frame_char(str, 0)) {
        unsigned char b3 = (unsigned char)str[2];
        return b3 == 0x8C;
    }
    
    std::string valid_starts = "lL[";
    for (char valid : valid_starts) {
        if (str[0] == valid) {
            return true;
        }
    }
    
    return false;
}

bool frame_matches_template(const std::string& template_str, const std::vector<std::string>& frame_symbols) {
    std::string frame_check_chars = "[]";
    
    for (size_t i = 0; i < frame_check_chars.length() && i < frame_symbols.size(); i++) {
        if (template_str.find(frame_check_chars[i]) != std::string::npos) {
            if (frame_symbols[i].empty()) {
                return false;
            }
        }
    }
    
    for (size_t i = 0; i < template_str.length(); ) {
        if (is_unicode_frame_char(template_str, i)) {
            unsigned char b3 = (unsigned char)template_str[i + 2];
            size_t frame_index = 0;
            
            if (b3 == 0x80) frame_index = 0;
            else if (b3 == 0x8C) frame_index = 1;
            else if (b3 == 0x94) frame_index = 2;
            
            if (frame_index < frame_symbols.size() && frame_symbols[frame_index].empty()) {
                return false;
            }
            
            i += 3;
        } else {
            i++;
        }
    }
    
    return true;
}

int main() {
    try {
        std::string username = get_username();
        
        if (username.empty()) {
            print_error(ERROR_USERNAME_NOT_FOUND);
            return ERROR_USERNAME_NOT_FOUND;
        }
        
        UserConfig config = get_user_config(username);
        
        if (config.template_string.empty()) {
            if (!add_new_user(username)) {
                print_error(ERROR_CONFIG_READ_FAILED);
                return ERROR_CONFIG_READ_FAILED;
            }
            config = get_user_config(username);
            
            if (config.template_string.empty()) {
                print_error(ERROR_TEMPLATE_INVALID);
                return ERROR_TEMPLATE_INVALID;
            }
        }
        
        if (config.frame_symbols.size() < 5) {
            print_error(ERROR_FRAME_SYMBOLS_INVALID);
            return ERROR_FRAME_SYMBOLS_INVALID;
        }
        
        for (const auto& symbol : config.frame_symbols) {
            if (!contains_only_allowed_chars(symbol)) {
                print_error(ERROR_FRAME_SYMBOLS_CONTENT);
                return ERROR_FRAME_SYMBOLS_CONTENT;
            }
        }
        
        if (!frame_matches_template(config.template_string, config.frame_symbols)) {
            print_error(ERROR_FRAME_TEMPLATE_MISMATCH);
            return ERROR_FRAME_TEMPLATE_MISMATCH;
        }
        
        if (!is_valid_tag(config.user_tag)) {
            print_error(ERROR_TAG_BRACKETS);
            return ERROR_TAG_BRACKETS;
        }
        
        if (!is_valid_tag(config.path_tag)) {
            print_error(ERROR_TAG_BRACKETS);
            return ERROR_TAG_BRACKETS;
        }
        
        if (!is_valid_tag(config.symbol_tag)) {
            print_error(ERROR_TAG_BRACKETS);
            return ERROR_TAG_BRACKETS;
        }
        
        if (config.user_tag.empty() || config.path_tag.empty() || config.symbol_tag.empty()) {
            print_error(ERROR_TAG_EMPTY);
            return ERROR_TAG_EMPTY;
        }
        
        if (!contains_only_allowed_chars(config.template_string)) {
            print_error(ERROR_TEMPLATE_INVALID);
            return ERROR_TEMPLATE_INVALID;
        }
        
        if (!has_valid_escape(config.template_string)) {
            print_error(ERROR_ESCAPE_INVALID);
            return ERROR_ESCAPE_INVALID;
        }
        
        if (!starts_with_valid_frame(config.template_string)) {
            print_error(ERROR_TEMPLATE_FORMAT_INVALID);
            return ERROR_TEMPLATE_FORMAT_INVALID;
        }
        
        std::string current_dir = get_current_dir();
        std::string symbol = config.symbol_tag;
        
        bool has_errors = false;
        
        if (config.template_string.find(config.user_tag) == std::string::npos) {
            print_error(ERROR_USER_TAG_MISMATCH);
            has_errors = true;
        }
        
        if (config.template_string.find(config.path_tag) == std::string::npos) {
            print_error(ERROR_PATH_TAG_MISMATCH);
            has_errors = true;
        }
        
        if (config.template_string.find(config.symbol_tag) == std::string::npos) {
            print_error(ERROR_SYMBOL_TAG_MISMATCH);
            has_errors = true;
        }
        
        std::string final_prompt = apply_template(config, username, current_dir, symbol);
        
        std::cout << final_prompt;
        
        return has_errors ? ERROR_UNKNOWN : SUCCESS;
        
    } catch (const std::exception& e) {
        print_error(ERROR_UNKNOWN);
        return ERROR_UNKNOWN;
    } catch (...) {
        print_error(ERROR_UNKNOWN);
        return ERROR_UNKNOWN;
    }
}