#include <iostream>
#include <string>
#include <cstdio>
#include "help.h"

namespace {
    const char* get_background_color_code(int color) {
        switch (color) {
            case 0: return "\033[40m";
            case 1: return "\033[41m";
            case 2: return "\033[42m";
            case 3: return "\033[43m";
            case 4: return "\033[44m";
            case 5: return "\033[45m";
            case 6: return "\033[46m";
            case 7: return "\033[47m";
            case 8: return "\033[100m";
            case 9: return "\033[101m";
            case 10: return "\033[102m";
            case 11: return "\033[103m";
            case 12: return "\033[104m";
            case 13: return "\033[105m";
            case 14: return "\033[106m";
            case 15: return "\033[107m";
            default: return "\033[0m";
        }
    }
    
    const char* get_text_color_for_background(int bg_color) {
        if (bg_color == 0 || bg_color == 8) {
            return "\033[37m";
        }
        return "\033[30m";
    }
    
    const char* get_bracket_color_code(int color) {
        switch (color) {
            case 0: return "\033[30m";
            case 1: return "\033[31m";
            case 2: return "\033[32m";
            case 3: return "\033[33m";
            case 4: return "\033[34m";
            case 5: return "\033[35m";
            case 6: return "\033[36m";
            case 7: return "\033[37m";
            case 8: return "\033[90m";
            case 9: return "\033[91m";
            case 10: return "\033[92m";
            case 11: return "\033[93m";
            case 12: return "\033[94m";
            case 13: return "\033[95m";
            case 14: return "\033[96m";
            case 15: return "\033[97m";
            default: return "\033[0m";
        }
    }
    
    void print_color_block(int num, const char* name, int width) {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%d - %s", num, name);
        
        std::string text = buffer;
        if (text.length() < width) {
            text += std::string(width - text.length(), ' ');
        }
        
        std::cout << get_background_color_code(num);
        std::cout << get_bracket_color_code(num);
        std::cout << "[";
        std::cout << get_text_color_for_background(num);
        std::cout << text;
        std::cout << get_bracket_color_code(num);
        std::cout << "]";
        std::cout << "\033[0m";
    }
}

void print_help() {
    std::cout << "███████████████████████████████████\n";
    std::cout << "█─▄▄▄─█─▄▄─█▄─▄███─▄▄─█▄─▄▄▀█─▄▄▄▄█\n";
    std::cout << "█─███▀█─██─██─██▀█─██─██─▄─▄█▄▄▄▄─█\n";
    std::cout << "▀▄▄▄▄▄▀▄▄▄▄▀▄▄▄▄▄▀▄▄▄▄▀▄▄▀▄▄▀▄▄▄▄▄▀\n\n";
    
    std::cout << "COLORC - CUSTOMIZABLE TERMINAL PROMPT GENERATOR\n\n";
    
    std::cout << "DESCRIPTION:\n";
    std::cout << "  colorc generates customizable terminal prompts with dynamic user,\n";
    std::cout << "  computer, path, and symbol replacement. Supports Unicode frame characters\n";
    std::cout << "  and flexible template configuration with multi-color frame support.\n\n";
    
    std::cout << "USAGE:\n";
    std::cout << "  colorc                    Generate prompt using default/user config\n";
    std::cout << "  colorc --help             Display this help message\n";
    std::cout << "  colorc -h                 Short form of --help\n\n";
    
    std::cout << "CONFIGURATION FILE:\n";
    std::cout << "  Location:    ~/.prompt_config\n";
    std::cout << "  Auto-create: Created automatically on first run with default config\n\n";
    
    std::cout << "CONFIGURATION SYNTAX:\n";
    std::cout << "  username: template_string\n";
    std::cout << "  frame1: symbol1;symbol2;symbol3;...\n";
    std::cout << "  colors_frame1: color_code\n";
    std::cout << "  frame2: symbol4;symbol5;... (optional, up to frame9)\n";
    std::cout << "  colors_frame2: color_code\n";
    std::cout << "  user: <user_tag>\n";
    std::cout << "  colors_user: color_code\n";
    std::cout << "  computer: <computer_tag>\n";
    std::cout << "  colors_computer: color_code\n";
    std::cout << "  path: <path_tag>\n";
    std::cout << "  colors_path: color_code\n";
    std::cout << "  symbol$: <symbol_tag>\n";
    std::cout << "  colors_symbol$: color_code\n\n";
    
    std::cout << "COLOR CODES:\n\n";
    
    const char* color_names[] = {
        "Black", "Red", "Green", "Yellow", "Blue", "Magenta", "Cyan", "White"
    };
    
    const char* bright_names[] = {
        "Bright Black", "Bright Red", "Bright Green", "Bright Yellow",
        "Bright Blue", "Bright Magenta", "Bright Cyan", "Bright White"
    };
    
    int left_width = 11;
    int right_width = 19;
    
    for (int i = 0; i < 8; i++) {
        std::cout << "  ";
        print_color_block(i, color_names[i], left_width);
        std::cout << "     ";
        print_color_block(i + 8, bright_names[i], right_width);
        std::cout << "\n";
    }
    std::cout << "\n";
    
    std::cout << "TAGS (Replace with actual values):\n";
    std::cout << "  <user>       Current username\n";
    std::cout << "  <computer>   Current computer hostname\n";
    std::cout << "  <path>       Current directory (abbreviated)\n";
    std::cout << "  <$>          Prompt symbol ($ for users, # for root) with automatic trailing space\n";
    std::cout << "  Custom tags can be any <tag_name> format\n\n";
    
    std::cout << "FRAME SYMBOLS:\n";
    std::cout << "  Defined in 'frame1:' through 'frame9:' lines, separated by semicolons\n";
    std::cout << "  Each frame section can have its own color via colors_frameN\n";
    std::cout << "  All frame symbols combined must match characters used in template\n";
    std::cout << "  Common symbols: ─ (U+2500), ┌ (U+250C), └ (U+2514), – (U+2013), [, ]\n\n";
    
    std::cout << "MULTI-COLOR FRAMES:\n";
    std::cout << "  You can split frame symbols into up to 9 sections, each with its own color:\n";
    std::cout << "  Example - three colors:\n";
    std::cout << "    frame1: ─;┌;└\n";
    std::cout << "    colors_frame1: 4    # Blue\n";
    std::cout << "    frame2: [;]\n";
    std::cout << "    colors_frame2: 2    # Green\n";
    std::cout << "    frame3: ;–\n";
    std::cout << "    colors_frame3: 1    # Red\n\n";
    
    std::cout << "PROMPT SYMBOL BEHAVIOR:\n";
    std::cout << "  The <$> tag is automatically replaced with the prompt symbol followed by a space.\n";
    std::cout << "  For regular users: '$ ' (dollar sign + space)\n";
    std::cout << "  For root user:     '# ' (hash sign + space)\n";
    std::cout << "  This ensures a clean separation between prompt and command input.\n\n";
    
    std::cout << "ESCAPE SEQUENCES:\n";
    std::cout << "  \\n        New line in template\n";
    std::cout << "  Only \\n is supported, other escapes are invalid\n\n";
    
    std::cout << "VALIDATION RULES:\n";
    std::cout << "  1. Template must contain all frame symbols at least once\n";
    std::cout << "  2. All frame-like characters in template must be in some frame section\n";
    std::cout << "  3. Each frame section must have a corresponding colors_frameN entry\n";
    std::cout << "  4. No symbol can appear in more than one frame section\n";
    std::cout << "  5. Frame sections must be defined in order (1,2,3...) without gaps\n";
    std::cout << "  6. Maximum of 9 frame sections (frame1 through frame9)\n";
    std::cout << "  7. Tags must be enclosed in angle brackets < >\n";
    std::cout << "  8. All color fields must be explicitly defined\n";
    std::cout << "  9. Color codes must be between 0 and 15\n\n";
    
    std::cout << "COMMON ERRORS:\n";
    std::cout << "  ERROR_FRAME_SECTION_EMPTY     - Frame section contains no symbols\n";
    std::cout << "  ERROR_FRAME_SYMBOL_DUPLICATE  - Symbol appears in multiple sections\n";
    std::cout << "  ERROR_FRAME_SECTIONS_LIMIT    - More than 9 frame sections defined\n";
    std::cout << "  ERROR_FRAME_COLOR_MISSING     - Frame section has no color defined\n";
    std::cout << "  ERROR_FRAME_INDEX_MISMATCH    - Sections not in order (1,2,3...)\n\n";
    
    std::cout << "COMMENTING:\n";
    std::cout << "  # comment                Full line comment\n";
    std::cout << "  key: value # comment     Inline comment (not recommended)\n\n";
    
    std::cout << "DEFAULT CONFIGURATION:\n";
    std::cout << "  default: ┌──[<user>–<computer>]──[<path>]\\n└──<$>\n";
    std::cout << "  frame1: ─;┌;└;[;];–\n";
    std::cout << "  colors_frame1: 4\n";
    std::cout << "  user: <user>\n";
    std::cout << "  colors_user: 6\n";
    std::cout << "  computer: <computer>\n";
    std::cout << "  colors_computer: 6\n";
    std::cout << "  path: <path>\n";
    std::cout << "  colors_path: 2\n";
    std::cout << "  symbol$: <$>\n";
    std::cout << "  colors_symbol$: 6\n\n";
    
    std::cout << "EXAMPLES:\n\n";
    
    std::cout << "  Example 1 - Single color frame (traditional):\n";
    std::cout << "    username: ┌──[<user>–<computer>]──[<path>]\\n└──<$>\n";
    std::cout << "    frame1: ─;┌;└;[;];–\n";
    std::cout << "    colors_frame1: 4\n";
    std::cout << "    user: <user>\n";
    std::cout << "    colors_user: 6\n";
    std::cout << "    computer: <computer>\n";
    std::cout << "    colors_computer: 6\n";
    std::cout << "    path: <path>\n";
    std::cout << "    colors_path: 2\n";
    std::cout << "    symbol$: <$>\n";
    std::cout << "    colors_symbol$: 6\n\n";
    
    std::cout << "  Example 2 - Multi-color frame with 3 sections:\n";
    std::cout << "    username: ┌──[<user>–<computer>]──[<path>]\\n└──<$>\n";
    std::cout << "    frame1: ─;┌;└\n";
    std::cout << "    colors_frame1: 4    # Blue\n";
    std::cout << "    frame2: [;]\n";
    std::cout << "    colors_frame2: 2    # Green\n";
    std::cout << "    frame3: ;–\n";
    std::cout << "    colors_frame3: 1    # Red\n";
    std::cout << "    user: <user>\n";
    std::cout << "    colors_user: 6\n";
    std::cout << "    computer: <computer>\n";
    std::cout << "    colors_computer: 6\n";
    std::cout << "    path: <path>\n";
    std::cout << "    colors_path: 2\n";
    std::cout << "    symbol$: <$>\n";
    std::cout << "    colors_symbol$: 6\n\n";
    
    std::cout << "  Example 3 - Custom symbols with 2 colors:\n";
    std::cout << "    username: ===[<user>–<computer>]===[<path>]\\n==><lol>\n";
    std::cout << "    frame1: =;[;];–\n";
    std::cout << "    colors_frame1: 5    # Magenta\n";
    std::cout << "    frame2: >;l;o\n";
    std::cout << "    colors_frame2: 3    # Yellow\n";
    std::cout << "    user: <user>\n";
    std::cout << "    colors_user: 3\n";
    std::cout << "    computer: <computer>\n";
    std::cout << "    colors_computer: 3\n";
    std::cout << "    path: <path>\n";
    std::cout << "    colors_path: 2\n";
    std::cout << "    symbol$: <lol>\n";
    std::cout << "    colors_symbol$: 1\n\n";
    
    std::cout << "AUTOMATIC USER CREATION:\n";
    std::cout << "  When a new user runs colorc, their configuration is automatically\n";
    std::cout << "  created based on the 'default' configuration if it doesn't exist.\n";
    std::cout << "  All frame sections and colors are copied from default configuration.\n\n";
    
    std::cout << "CACHE SYSTEM:\n";
    std::cout << "  Configurations are cached for performance. Cache is invalidated\n";
    std::cout << "  when configuration file is modified. Use reload if needed.\n\n";
    
    std::cout << "SEE ALSO:\n";
    std::cout << "  Configuration validation occurs automatically. For debugging,\n";
    std::cout << "  check ~/.prompt_config syntax and ensure all frame sections are valid.\n";
}