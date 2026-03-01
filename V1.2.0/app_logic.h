#ifndef APP_LOGIC_H
#define APP_LOGIC_H

#include <string>
#include <vector>
#include "config_main.h"
#include "error.h"

ErrorCode generate_prompt(std::string& output_prompt);
ErrorCode validate_user_config(const std::string& username);
ErrorCode add_user_if_not_exists(const std::string& username);
ErrorCode get_all_config_users(std::vector<std::string>& users);
ErrorCode update_user_prompt(const std::string& username, const UserConfig& new_config);
ErrorCode delete_user_prompt(const std::string& username);
void print_prompt(const std::string& prompt);

#endif