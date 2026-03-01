#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <vector>
#include <map>
#include "config_main.h"

std::map<std::string, UserConfig> parse_config_file(const std::string& path);
UserConfig parse_user_section(const std::vector<std::string>& lines);
std::vector<std::string> split(const std::string& str, char delimiter);
bool save_user_config(const std::string& path, const std::string& username, const UserConfig& config);
bool update_user_config_in_file(const std::string& path, const std::string& username, const UserConfig& config);

#endif