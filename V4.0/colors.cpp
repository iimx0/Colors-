#include "colors.h"
#include "config_main.h"
#include <string>
#include <sstream>
#include <vector>
#include <map>

namespace {
    const char* get_ansi_color_code(int color) {
        switch (color) {
            case 0: return "\001\033[30m\002";
            case 1: return "\001\033[31m\002";
            case 2: return "\001\033[32m\002";
            case 3: return "\001\033[33m\002";
            case 4: return "\001\033[34m\002";
            case 5: return "\001\033[35m\002";
            case 6: return "\001\033[36m\002";
            case 7: return "\001\033[37m\002";
            case 8: return "\001\033[90m\002";
            case 9: return "\001\033[91m\002";
            case 10: return "\001\033[92m\002";
            case 11: return "\001\033[93m\002";
            case 12: return "\001\033[94m\002";
            case 13: return "\001\033[95m\002";
            case 14: return "\001\033[96m\002";
            case 15: return "\001\033[97m\002";
            default: return "\001\033[0m\002";
        }
    }
    
    std::string wrap_with_color(const std::string& text, int color_code) {
        if (text.empty() || color_code < 0 || color_code > 15) {
            return text;
        }
        return std::string(get_ansi_color_code(color_code)) + text + "\001\033[0m\002";
    }
    
    struct ColorSection {
        size_t start;
        size_t end;
        int color;
    };
    
    void add_section(std::vector<ColorSection>& sections, size_t start, size_t end, int color) {
        if (start < end) {
            ColorSection section;
            section.start = start;
            section.end = end;
            section.color = color;
            sections.push_back(section);
        }
    }
    
    int find_color_for_symbol(const std::string& symbol, const UserConfig& config) {
        for (const auto& section : config.frame_sections) {
            for (const auto& s : section.symbols) {
                if (s == symbol) {
                    return section.color;
                }
            }
        }
        return -1;
    }
}

bool validate_colors_config(const UserConfig& config, ErrorCode& error) {
    if (config.colors_user == -1) {
        error = ERROR_COLOR_MISSING;
        return false;
    }
    if (config.colors_computer == -1) {
        error = ERROR_COLOR_MISSING;
        return false;
    }
    if (config.colors_path == -1) {
        error = ERROR_COLOR_MISSING;
        return false;
    }
    if (config.colors_symbol == -1) {
        error = ERROR_COLOR_MISSING;
        return false;
    }
    
    if (config.colors_user < 0 || config.colors_user > 15) {
        error = ERROR_COLOR_INVALID;
        return false;
    }
    if (config.colors_computer < 0 || config.colors_computer > 15) {
        error = ERROR_COLOR_INVALID;
        return false;
    }
    if (config.colors_path < 0 || config.colors_path > 15) {
        error = ERROR_COLOR_INVALID;
        return false;
    }
    if (config.colors_symbol < 0 || config.colors_symbol > 15) {
        error = ERROR_COLOR_INVALID;
        return false;
    }
    
    for (const auto& section : config.frame_sections) {
        if (section.color < 0 || section.color > 15) {
            error = ERROR_COLOR_INVALID;
            return false;
        }
    }
    
    error = SUCCESS;
    return true;
}

std::string color_code_to_ansi(int color_code) {
    return std::string(get_ansi_color_code(color_code));
}

std::string apply_colors_to_prompt(
    const std::string& prompt,
    const UserConfig& config,
    const std::string& username,
    const std::string& computer_name,
    const std::string& current_dir,
    const std::string& symbol
) {
    std::vector<ColorSection> sections;
    std::string result = prompt;
    
    size_t user_pos = result.find(username);
    size_t computer_pos = result.find(computer_name);
    size_t path_pos = result.find(current_dir);
    size_t symbol_pos = result.find(symbol);
    
    if (user_pos != std::string::npos) {
        add_section(sections, user_pos, user_pos + username.length(), config.colors_user);
    }
    
    if (computer_pos != std::string::npos) {
        add_section(sections, computer_pos, computer_pos + computer_name.length(), config.colors_computer);
    }
    
    if (path_pos != std::string::npos) {
        add_section(sections, path_pos, path_pos + current_dir.length(), config.colors_path);
    }
    
    if (symbol_pos != std::string::npos) {
        add_section(sections, symbol_pos, symbol_pos + symbol.length(), config.colors_symbol);
    }
    
    std::map<size_t, std::pair<std::string, int>> frame_symbol_positions;
    
    for (size_t i = 0; i < result.length(); ) {
        bool found = false;
        
        for (const auto& section : config.frame_sections) {
            for (const auto& frame_symbol : section.symbols) {
                if (result.compare(i, frame_symbol.length(), frame_symbol) == 0) {
                    frame_symbol_positions[i] = std::make_pair(frame_symbol, section.color);
                    i += frame_symbol.length();
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        
        if (!found) {
            i++;
        }
    }
    
    for (const auto& pos_pair : frame_symbol_positions) {
        size_t pos = pos_pair.first;
        const std::string& frame_symbol = pos_pair.second.first;
        int color = pos_pair.second.second;
        add_section(sections, pos, pos + frame_symbol.length(), color);
    }
    
    for (size_t i = 0; i < sections.size(); ++i) {
        for (size_t j = i + 1; j < sections.size(); ++j) {
            if (sections[i].start > sections[j].start) {
                ColorSection temp = sections[i];
                sections[i] = sections[j];
                sections[j] = temp;
            }
        }
    }
    
    std::string colored_result;
    size_t current_pos = 0;
    
    for (const auto& section : sections) {
        if (section.start > current_pos) {
            colored_result += result.substr(current_pos, section.start - current_pos);
        }
        colored_result += wrap_with_color(result.substr(section.start, section.end - section.start), section.color);
        current_pos = section.end;
    }
    
    if (current_pos < result.length()) {
        colored_result += result.substr(current_pos);
    }
    
    return colored_result;
}