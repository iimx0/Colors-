#ifndef PROMPT_H
#define PROMPT_H

#include <string>

std::string get_username();
std::string get_current_dir();
std::string get_symbol(const std::string& username);

#endif