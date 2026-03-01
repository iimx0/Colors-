#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <string>
#include "config.h"

std::string apply_template(const UserConfig& config, const std::string& username, const std::string& current_dir, const std::string& symbol);

#endif