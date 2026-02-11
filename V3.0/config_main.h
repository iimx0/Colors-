#ifndef CONFIG_MAIN_H
#define CONFIG_MAIN_H

#include <string>
#include <vector>

struct UserConfig {
    std::string template_string;
    std::vector<std::string> frame_symbols;
    std::string user_tag;
    std::string computer_tag;  
    std::string path_tag;
    std::string symbol_tag;
};

UserConfig get_user_config(const std::string& username);
bool add_new_user(const std::string& username);
bool update_user_config(const std::string& username, const UserConfig& config);
bool delete_user_config(const std::string& username);
void reload_config();
std::vector<std::string> get_all_users();
bool has_user_config(const std::string& username);

#endif