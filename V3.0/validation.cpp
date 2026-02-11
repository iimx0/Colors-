#include "validation.h"
#include "config_main.h"
#include "config_validator.h"
#include <cctype>
#include <algorithm>
#include <set>

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
        return true;
    }
    
    return true;
}

bool frame_matches_template(const std::string& template_str, const std::vector<std::string>& frame_symbols) {
    if (frame_symbols.empty()) {
        return false;
    }
    
    std::set<std::string> frame_symbols_set;
    for (const auto& symbol : frame_symbols) {
        if (!symbol.empty()) {
            frame_symbols_set.insert(symbol);
        }
    }
    
    std::set<std::string> template_symbols_set;
    
    for (size_t i = 0; i < template_str.length(); ) {
        if (template_str[i] == '<') {
            size_t end_pos = template_str.find('>', i);
            if (end_pos != std::string::npos) {
                std::string between = template_str.substr(i + 1, end_pos - i - 1);
                bool valid_tag = true;
                
                for (char c : between) {
                    if (!std::isalnum(c) && c != '_' && c != '$') {
                        valid_tag = false;
                        break;
                    }
                }
                
                if (valid_tag && !between.empty()) {
                    i = end_pos + 1;
                    continue;
                }
            }
        }
        
        if (template_str[i] == '\\' && i + 1 < template_str.length() && template_str[i + 1] == 'n') {
            i += 2;
            continue;
        }
        
        bool found_symbol = false;
        for (const auto& symbol : frame_symbols_set) {
            if (template_str.compare(i, symbol.length(), symbol) == 0) {
                template_symbols_set.insert(symbol);
                i += symbol.length();
                found_symbol = true;
                break;
            }
        }
        
        if (!found_symbol) {
            if (is_unicode_frame_char(template_str, i)) {
                std::string unicode_char = template_str.substr(i, 3);
                template_symbols_set.insert(unicode_char);
                i += 3;
            } else {
                std::string single_char(1, template_str[i]);
                template_symbols_set.insert(single_char);
                i++;
            }
        }
    }
    
    if (frame_symbols_set.size() != template_symbols_set.size()) {
        return false;
    }
    
    for (const auto& symbol : frame_symbols_set) {
        if (template_symbols_set.find(symbol) == template_symbols_set.end()) {
            return false;
        }
    }
    
    for (const auto& symbol : template_symbols_set) {
        if (frame_symbols_set.find(symbol) == frame_symbols_set.end()) {
            return false;
        }
    }
    
    return true;
}

bool validate_full_config(const UserConfig& config, ErrorCode& error) {
    error = SUCCESS;
    
    if (config.template_string.empty()) {
        error = ERROR_TEMPLATE_INVALID;
        return false;
    }
    
    if (config.frame_symbols.empty()) {
        error = ERROR_FRAME_SYMBOLS_INVALID;
        return false;
    }
    
    for (const auto& symbol : config.frame_symbols) {
        if (symbol.empty()) {
            error = ERROR_FRAME_SYMBOLS_CONTENT;
            return false;
        }
        if (!contains_only_allowed_chars(symbol)) {
            error = ERROR_FRAME_SYMBOLS_CONTENT;
            return false;
        }
    }
    
    if (!validate_template_string(config.template_string)) {
        error = ERROR_TEMPLATE_INVALID;
        return false;
    }
    
    if (!config.user_tag.empty() && !validate_tag(config.user_tag)) {
        error = ERROR_TAG_BRACKETS;
        return false;
    }
    
    if (!config.computer_tag.empty() && !validate_tag(config.computer_tag)) {
        error = ERROR_TAG_BRACKETS;
        return false;
    }
    
    if (!config.path_tag.empty() && !validate_tag(config.path_tag)) {
        error = ERROR_TAG_BRACKETS;
        return false;
    }
    
    if (!config.symbol_tag.empty() && !validate_tag(config.symbol_tag)) {
        error = ERROR_TAG_BRACKETS;
        return false;
    }
    
    if (!frame_matches_template(config.template_string, config.frame_symbols)) {
        error = ERROR_FRAME_TEMPLATE_MISMATCH;
        return false;
    }
    
    if (!has_valid_escape(config.template_string)) {
        error = ERROR_ESCAPE_INVALID;
        return false;
    }
    
    if (!starts_with_valid_frame(config.template_string)) {
        error = ERROR_TEMPLATE_FORMAT_INVALID;
        return false;
    }
    
    if (!config.user_tag.empty() && config.template_string.find(config.user_tag) == std::string::npos) {
        error = ERROR_USER_TAG_MISMATCH;
        return false;
    }
    
    if (!config.computer_tag.empty() && config.template_string.find(config.computer_tag) == std::string::npos) {
        error = ERROR_COMPUTER_TAG_MISMATCH;
        return false;
    }
    
    if (!config.path_tag.empty() && config.template_string.find(config.path_tag) == std::string::npos) {
        error = ERROR_PATH_TAG_MISMATCH;
        return false;
    }
    
    if (!config.symbol_tag.empty() && config.template_string.find(config.symbol_tag) == std::string::npos) {
        error = ERROR_SYMBOL_TAG_MISMATCH;
        return false;
    }
    
    return true;
}

bool is_unicode_frame_char(const std::string& str, size_t pos) {
    if (pos + 2 >= str.length()) return false;
    
    unsigned char b1 = (unsigned char)str[pos];
    unsigned char b2 = (unsigned char)str[pos + 1];
    unsigned char b3 = (unsigned char)str[pos + 2];
    
    if (b1 == 0xE2 && b2 == 0x94) {
        return b3 == 0x80 || b3 == 0x8C || b3 == 0x94;
    }
    
    if (b1 == 0xE2 && b2 == 0x80) {
        return b3 == 0x93 || b3 == 0x94;
    }
    
    return false;
}