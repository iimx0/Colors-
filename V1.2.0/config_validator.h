#ifndef CONFIG_VALIDATOR_H
#define CONFIG_VALIDATOR_H

#include <string>
#include <vector>
#include "config_main.h"
#include "error.h"

bool validate_user_config(const UserConfig& config, ErrorCode& error);
bool validate_frame_symbols(const std::vector<std::string>& symbols);
bool validate_template_string(const std::string& template_str);
bool validate_tag(const std::string& tag);
bool contains_only_allowed_chars(const std::string& str);
bool is_unicode_frame_char(const std::string& str, size_t pos);
bool is_valid_tag_string(const std::string& tag);

#endif