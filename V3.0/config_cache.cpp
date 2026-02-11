#include "config_cache.h"
#include <map>
#include <ctime>
#include <sys/stat.h>

static std::map<std::string, UserConfig>& get_config_cache() {
    static std::map<std::string, UserConfig> config_cache;
    return config_cache;
}

static std::time_t& get_last_modified_time() {
    static std::time_t last_modified_time = 0;
    return last_modified_time;
}

static std::string& get_cached_config_path() {
    static std::string cached_config_path;
    return cached_config_path;
}

void cache_user_config(const std::string& username, const UserConfig& config) {
    get_config_cache()[username] = config;
}

bool get_cached_config(const std::string& username, UserConfig& out_config) {
    auto& cache = get_config_cache();
    auto it = cache.find(username);
    if (it != cache.end()) {
        out_config = it->second;
        return true;
    }
    return false;
}

void clear_cache() {
    get_config_cache().clear();
    get_last_modified_time() = 0;
    get_cached_config_path().clear();
}

bool is_cache_valid(const std::string& config_path) {
    if (config_path != get_cached_config_path()) {
        return false;
    }
    
    if (get_last_modified_time() == 0) {
        return false;
    }
    
    struct stat file_stat;
    if (stat(config_path.c_str(), &file_stat) != 0) {
        return false;
    }
    
    return file_stat.st_mtime <= get_last_modified_time();
}

void update_cache_timestamp(const std::string& config_path) {
    struct stat file_stat;
    if (stat(config_path.c_str(), &file_stat) == 0) {
        get_last_modified_time() = file_stat.st_mtime;
        get_cached_config_path() = config_path;
    }
}

bool has_config_in_cache(const std::string& username) {
    return get_config_cache().find(username) != get_config_cache().end();
}

void remove_from_cache(const std::string& username) {
    get_config_cache().erase(username);
}

std::vector<std::string> get_cached_users() {
    std::vector<std::string> users;
    auto& cache = get_config_cache();
    for (const auto& pair : cache) {
        users.push_back(pair.first);
    }
    return users;
}

void invalidate_cache() {
    get_last_modified_time() = 0;
}