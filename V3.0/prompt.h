#ifndef PROMPT_H
#define PROMPT_H

#include <string>
#include "config_main.h"

std::string get_username();
std::string get_current_dir();
std::string get_computer_name();
std::string get_symbol(const std::string& username, const UserConfig& config);

#endif