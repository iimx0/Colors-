#ifndef CONFIG_CACHE_H
#define CONFIG_CACHE_H

#include <string>
#include <vector>
#include "config_main.h"

void cache_user_config(const std::string& username, const UserConfig& config);
bool get_cached_config(const std::string& username, UserConfig& out_config);
void clear_cache();
bool is_cache_valid(const std::string& config_path);
void update_cache_timestamp(const std::string& config_path);
bool has_config_in_cache(const std::string& username);
void remove_from_cache(const std::string& username);
std::vector<std::string> get_cached_users();
void invalidate_cache();

#endif