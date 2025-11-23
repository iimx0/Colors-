#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

struct UserConfig {
    std::string template_string;
    std::vector<std::string> frame_symbols;
    std::string user_tag;
    std::string path_tag;
    std::string symbol_tag;
};

std::string get_config_path();
bool file_exists(const std::string& path);
void create_default_config();
std::vector<std::string> split(const std::string& str, char delimiter);
UserConfig parse_user_config(const std::vector<std::string>& lines);
void load_config();
UserConfig get_user_config(const std::string& username);
bool add_new_user(const std::string& username);

#endif