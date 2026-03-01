#include <iostream>
#include <string>
#include "help.h"

void print_help() {
    std::cout << "███████████████████████████████████\n";
    std::cout << "█─▄▄▄─█─▄▄─█▄─▄███─▄▄─█▄─▄▄▀█─▄▄▄▄█\n";
    std::cout << "█─███▀█─██─██─██▀█─██─██─▄─▄█▄▄▄▄─█\n";
    std::cout << "▀▄▄▄▄▄▀▄▄▄▄▀▄▄▄▄▄▀▄▄▄▄▀▄▄▀▄▄▀▄▄▄▄▄▀\n\n";
    
    std::cout << "COLORC - CUSTOMIZABLE TERMINAL PROMPT GENERATOR\n\n";
    
    std::cout << "DESCRIPTION:\n";
    std::cout << "  colorc generates customizable terminal prompts with dynamic user,\n";
    std::cout << "  computer, path, and symbol replacement. Supports Unicode frame characters\n";
    std::cout << "  and flexible template configuration.\n\n";
    
    std::cout << "USAGE:\n";
    std::cout << "  colorc                    Generate prompt using default/user config\n";
    std::cout << "  colorc --help             Display this help message\n";
    std::cout << "  colorc -h                 Short form of --help\n\n";
    
    std::cout << "CONFIGURATION FILE:\n";
    std::cout << "  Location:    ~/.prompt_config\n";
    std::cout << "  Auto-create: Created automatically on first run with default config\n\n";
    
    std::cout << "CONFIGURATION SYNTAX:\n";
    std::cout << "  username: template_string\n";
    std::cout << "  frame: symbol1;symbol2;symbol3;...\n";
    std::cout << "  user: <user_tag>\n";
    std::cout << "  computer: <computer_tag>\n";
    std::cout << "  path: <path_tag>\n";
    std::cout << "  symbol$: <symbol_tag>\n\n";
    
    std::cout << "TAGS (Replace with actual values):\n";
    std::cout << "  <user>       Current username\n";
    std::cout << "  <computer>   Current computer hostname\n";
    std::cout << "  <path>       Current directory (abbreviated)\n";
    std::cout << "  <$>          Prompt symbol ($ for users, # for root)\n";
    std::cout << "  Custom tags can be any <tag_name> format\n\n";
    
    std::cout << "FRAME SYMBOLS:\n";
    std::cout << "  Defined in 'frame:' line, separated by semicolons\n";
    std::cout << "  Must match characters used in template (bidirectional validation)\n";
    std::cout << "  Common symbols: ─ (U+2500), ┌ (U+250C), └ (U+2514), – (U+2013), [, ]\n\n";
    
    std::cout << "ESCAPE SEQUENCES:\n";
    std::cout << "  \\n        New line in template\n";
    std::cout << "  Only \\n is supported, other escapes are invalid\n\n";
    
    std::cout << "VALIDATION RULES:\n";
    std::cout << "  1. Template must contain all frame symbols at least once\n";
    std::cout << "  2. All frame-like characters in template must be in frame list\n";
    std::cout << "  3. Tags must be enclosed in angle brackets < >\n";
    std::cout << "  4. Tags can contain letters, numbers, _, $\n";
    std::cout << "  5. Empty tags or invalid characters cause validation errors\n";
    std::cout << "  6. 'computer:' tag is required in configuration\n\n";
    
    std::cout << "COMMENTING:\n";
    std::cout << "  # comment                Full line comment\n";
    std::cout << "  key: value # comment     Inline comment (not recommended)\n\n";
    
    std::cout << "DEFAULT CONFIGURATION:\n";
    std::cout << "  default: ┌──[<user>–<computer>]──[<path>]\\n└──<$>\n";
    std::cout << "  frame: ─;┌;└;[;];–\n";
    std::cout << "  user: <user>\n";
    std::cout << "  computer: <computer>\n";
    std::cout << "  path: <path>\n";
    std::cout << "  symbol$: <$>\n\n";
    
    std::cout << "EXAMPLES:\n\n";
    
    std::cout << "  Example 1 - Basic configuration with computer hostname:\n";
    std::cout << "    username: ┌──[<user>–<computer>]──[<path>]\\n└──<$>\n";
    std::cout << "    frame: ─;┌;└;[;];–\n";
    std::cout << "    user: <user>\n";
    std::cout << "    computer: <computer>\n";
    std::cout << "    path: <path>\n";
    std::cout << "    symbol$: <$>\n\n";
    
    std::cout << "  Example 2 - Custom symbols:\n";
    std::cout << "    username: ===[<user>–<computer>]===[<path>]\\n==><lol>\n";
    std::cout << "    frame: =;[;];–;>;l;o\n";
    std::cout << "    user: <user>\n";
    std::cout << "    computer: <computer>\n";
    std::cout << "    path: <path>\n";
    std::cout << "    symbol$: <lol>\n\n";
    
    std::cout << "  Example 3 - Disabled field (commented):\n";
    std::cout << "    username: ┌──[<user>–<computer>]──[<path>]\\n└──<$>\n";
    std::cout << "    frame: ─;┌;└;[;];–\n";
    std::cout << "    # user: <user>          # Tag <user> won't be replaced\n";
    std::cout << "    computer: <computer>\n";
    std::cout << "    path: <path>\n";
    std::cout << "    symbol$: <$>\n\n";
    
    std::cout << "ERROR CODES:\n";
    std::cout << "  See error messages for specific error codes and descriptions\n";
    std::cout << "  Common errors include frame/template mismatch, invalid tags,\n";
    std::cout << "  missing computer tag, escape sequence errors, and configuration parsing failures.\n\n";
    
    std::cout << "AUTOMATIC USER CREATION:\n";
    std::cout << "  When a new user runs colorc, their configuration is automatically\n";
    std::cout << "  created based on the 'default' configuration if it doesn't exist.\n\n";
    
    std::cout << "CACHE SYSTEM:\n";
    std::cout << "  Configurations are cached for performance. Cache is invalidated\n";
    std::cout << "  when configuration file is modified. Use reload if needed.\n\n";
    
    std::cout << "DEVELOPMENT:\n";
    std::cout << "  Tool is designed for extensibility. Future features may include\n";
    std::cout << "  color support, additional dynamic variables, and plugin system.\n\n";
    
    std::cout << "SEE ALSO:\n";
    std::cout << "  Configuration validation occurs automatically. For debugging,\n";
    std::cout << "  check ~/.prompt_config syntax and ensure frame symbols match.\n";
}