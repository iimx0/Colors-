#ifndef CONFIG_UTILS_H
#define CONFIG_UTILS_H

#include <string>

std::string get_config_path();
bool file_exists(const std::string& path);
bool create_default_config(const std::string& path);
std::string read_file_content(const std::string& path);
bool write_file_content(const std::string& path, const std::string& content);
bool backup_config_file(const std::string& path);
std::string get_home_directory();

#endif