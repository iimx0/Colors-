#ifndef COLORS_H
#define COLORS_H

#include <string>
#include "config_main.h"
#include "error.h"

bool validate_colors_config(const UserConfig& config, ErrorCode& error);
std::string color_code_to_ansi(int color_code);
std::string apply_colors_to_prompt(
    const std::string& prompt,
    const UserConfig& config,
    const std::string& username,
    const std::string& computer_name,
    const std::string& current_dir,
    const std::string& symbol
);

#endif