#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
#include <vector>
#include "config_main.h"
#include "error.h"

bool is_valid_tag(const std::string& tag);
bool has_valid_escape(const std::string& str);
bool starts_with_valid_frame(const std::string& str);
bool frame_matches_template(const std::string& template_str, const std::vector<std::string>& frame_symbols);
bool validate_full_config(const UserConfig& config, ErrorCode& error);

#endif