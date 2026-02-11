#include "config_validator.h"
#include "validation.h"
#include "config_main.h"
#include "error.h"
#include <cctype>
#include <algorithm>

bool validate_user_config(const UserConfig& config, ErrorCode& error) {
    error = SUCCESS;
    
    if (config.template_string.empty()) {
        error = ERROR_TEMPLATE_INVALID;
        return false;
    }
    
    if (config.frame_symbols.size() < 1) {
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
    
    if (config.user_tag.empty() || config.path_tag.empty() || config.symbol_tag.empty()) {
        error = ERROR_TAG_EMPTY;
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
    
    if (config.template_string.find(config.user_tag) == std::string::npos) {
        error = ERROR_USER_TAG_MISMATCH;
        return false;
    }
    
    if (!config.computer_tag.empty() && 
        config.template_string.find(config.computer_tag) == std::string::npos) {
        error = ERROR_COMPUTER_TAG_MISMATCH;
        return false;
    }
    
    if (config.template_string.find(config.path_tag) == std::string::npos) {
        error = ERROR_PATH_TAG_MISMATCH;
        return false;
    }
    
    if (config.template_string.find(config.symbol_tag) == std::string::npos) {
        error = ERROR_SYMBOL_TAG_MISMATCH;
        return false;
    }
    
    return true;
}

bool validate_frame_symbols(const std::vector<std::string>& symbols) {
    if (symbols.empty()) return false;
    
    for (const auto& symbol : symbols) {
        if (symbol.empty()) return false;
        if (!contains_only_allowed_chars(symbol)) return false;
    }
    
    return true;
}

bool validate_template_string(const std::string& template_str) {
    if (template_str.empty()) return false;
    
    if (!contains_only_allowed_chars(template_str)) return false;
    
    size_t escape_pos = 0;
    while ((escape_pos = template_str.find('\\', escape_pos)) != std::string::npos) {
        if (escape_pos + 1 >= template_str.length()) return false;
        if (template_str[escape_pos + 1] != 'n') return false;
        escape_pos += 2;
    }
    
    return true;
}

bool validate_tag(const std::string& tag) {
    if (tag.length() < 3) return false;
    if (tag[0] != '<' || tag[tag.length() - 1] != '>') return false;
    
    for (size_t i = 1; i < tag.length() - 1; i++) {
        char c = tag[i];
        if (c == '<' || c == '>') return false;
        if (!std::isprint(c)) return false;
    }
    
    return true;
}

bool contains_only_allowed_chars(const std::string& str) {
    for (size_t i = 0; i < str.length(); ) {
        if (is_unicode_frame_char(str, i)) {
            i += 3;
            continue;
        }
        
        char c = str[i];
        if (!std::isprint(c) && c != '\n' && c != '\t') {
            return false;
        }
        i++;
    }
    return true;
}

bool is_valid_tag_string(const std::string& tag) {
    return validate_tag(tag);
}