#include "app_logic.h"
#include "config_main.h"
#include "validation.h"
#include "prompt.h"
#include "template.h"
#include "error.h"
#include <iostream>

ErrorCode generate_prompt(std::string& output_prompt) {
    try {
        std::string username = get_username();
        
        if (username.empty()) {
            return ERROR_USERNAME_NOT_FOUND;
        }
        
        if (!has_user_config(username)) {
            if (!add_new_user(username)) {
                UserConfig default_config = get_user_config("default");
                if (default_config.template_string.empty()) {
                    return ERROR_CONFIG_READ_FAILED;
                }
                
                ErrorCode error;
                if (!validate_full_config(default_config, error)) {
                    return error;
                }
                
                std::string current_dir = get_current_dir();
                std::string symbol = get_symbol(username, default_config);
                
                output_prompt = apply_template(default_config, username, current_dir, symbol);
                return SUCCESS;
            }
        }
        
        UserConfig config = get_user_config(username);
        
        if (config.template_string.empty()) {
            UserConfig default_config = get_user_config("default");
            if (default_config.template_string.empty()) {
                return ERROR_TEMPLATE_INVALID;
            }
            config = default_config;
        }
        
        ErrorCode validation_error;
        if (!validate_full_config(config, validation_error)) {
            return validation_error;
        }
        
        std::string current_dir = get_current_dir();
        std::string symbol = get_symbol(username, config);
        
        output_prompt = apply_template(config, username, current_dir, symbol);
        
        return SUCCESS;
        
    } catch (const std::exception& e) {
        return ERROR_UNKNOWN;
    } catch (...) {
        return ERROR_UNKNOWN;
    }
}

ErrorCode validate_user_config(const std::string& username) {
    if (username.empty()) {
        return ERROR_USERNAME_NOT_FOUND;
    }
    
    UserConfig config = get_user_config(username);
    
    if (config.template_string.empty()) {
        return ERROR_TEMPLATE_INVALID;
    }
    
    ErrorCode error;
    if (!validate_full_config(config, error)) {
        return error;
    }
    
    return SUCCESS;
}

ErrorCode add_user_if_not_exists(const std::string& username) {
    if (username.empty()) {
        return ERROR_USERNAME_NOT_FOUND;
    }
    
    if (has_user_config(username)) {
        return SUCCESS;
    }
    
    if (!add_new_user(username)) {
        return ERROR_CONFIG_READ_FAILED;
    }
    
    return SUCCESS;
}

ErrorCode get_all_config_users(std::vector<std::string>& users) {
    try {
        users = get_all_users();
        return SUCCESS;
    } catch (...) {
        return ERROR_CONFIG_READ_FAILED;
    }
}

ErrorCode update_user_prompt(const std::string& username, const UserConfig& new_config) {
    if (username.empty()) {
        return ERROR_USERNAME_NOT_FOUND;
    }
    
    ErrorCode validation_error;
    if (!validate_full_config(new_config, validation_error)) {
        return validation_error;
    }
    
    if (!update_user_config(username, new_config)) {
        return ERROR_CONFIG_READ_FAILED;
    }
    
    return SUCCESS;
}

ErrorCode delete_user_prompt(const std::string& username) {
    if (username.empty()) {
        return ERROR_USERNAME_NOT_FOUND;
    }
    
    if (username == "default") {
        return ERROR_CONFIG_PERMISSION_DENIED;
    }
    
    if (!delete_user_config(username)) {
        return ERROR_CONFIG_READ_FAILED;
    }
    
    return SUCCESS;
}

void print_prompt(const std::string& prompt) {
    std::cout << prompt;
}