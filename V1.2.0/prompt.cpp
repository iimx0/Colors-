#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <pwd.h>
#include <sys/utsname.h>
#include "prompt.h"
#include "config_main.h"

std::string get_username() {
    const char* user = std::getenv("USER");
    if (user) return user;
    
    struct passwd* pw = getpwuid(geteuid());
    if (pw && pw->pw_name) return pw->pw_name;
    
    return "";
}

std::string get_current_dir() {
    char buffer[4096];
    if (getcwd(buffer, sizeof(buffer)) == nullptr) {
        return "?";
    }
    
    std::string path = buffer;
    const char* home = std::getenv("HOME");
    if (home && path.find(home) == 0) {
        path.replace(0, strlen(home), "~");
    }
    
    const int max_length = 30;
    
    if (path.length() > max_length) {
        path = "..." + path.substr(path.length() - max_length + 3);
    }
    
    return path;
}

std::string get_computer_name() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return std::string(hostname);
    }
    
    struct utsname uname_data;
    if (uname(&uname_data) == 0) {
        return std::string(uname_data.nodename);
    }
    
    const char* env_host = std::getenv("HOSTNAME");
    if (env_host) {
        return std::string(env_host);
    }
    
    const char* env_computer = std::getenv("COMPUTERNAME");
    if (env_computer) {
        return std::string(env_computer);
    }
    
    return "(computer)";
}

std::string get_symbol(const std::string& username, const UserConfig& config) {
    if (config.symbol_tag.empty()) {
        return (username == "root") ? "#" : "$";
    }
    
    if (config.symbol_tag.length() >= 3 && 
        config.symbol_tag[0] == '<' && 
        config.symbol_tag.back() == '>') {
        std::string tag_content = config.symbol_tag.substr(1, config.symbol_tag.length() - 2);
        
        if (tag_content == "$") {
            return (username == "root") ? "#" : "$";
        }
        
        return tag_content;
    }
    
    return config.symbol_tag;
}