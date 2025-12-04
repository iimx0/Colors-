#include <iostream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <pwd.h>
#include "prompt.h"

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

std::string get_symbol(const std::string& username) {
    return (username == "root") ? "#" : "$";
}