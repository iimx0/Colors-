#include <iostream>
#include <string>
#include "error.h"

const char* get_error_message(ErrorCode code) {
    switch (code) {
        case SUCCESS:
            return "Operation completed successfully";
        case ERROR_USERNAME_NOT_FOUND:
            return "Unable to retrieve current username from system";
        case ERROR_CONFIG_PERMISSION_DENIED:
            return "Insufficient permissions to access configuration file";
        case ERROR_CONFIG_READ_FAILED:
            return "Failed to read configuration file - file may be corrupted or inaccessible";
        case ERROR_CONFIG_PARSE_FAILED:
            return "Configuration file contains invalid syntax or format errors";
        case ERROR_FRAME_SYMBOLS_INVALID:
            return "Frame symbols configuration is incomplete or malformed";
        case ERROR_FRAME_SYMBOLS_CONTENT:
            return "Frame symbols contain invalid or non-printable characters";
        case ERROR_FRAME_TEMPLATE_MISMATCH:
            return "Frame symbols do not match characters used in template";
        case ERROR_USER_TAG_MISMATCH:
            return "User tag defined in config does not match any tag in the template";
        case ERROR_PATH_TAG_MISMATCH:
            return "Path tag defined in config does not match any tag in the template";
        case ERROR_SYMBOL_TAG_MISMATCH:
            return "Symbol tag defined in config does not match any tag in the template";
        case ERROR_COMPUTER_TAG_MISMATCH:
            return "Computer tag defined in config does not match any tag in the template";
        case ERROR_TEMPLATE_INVALID:
            return "Template string is empty or contains invalid characters";
        case ERROR_TEMPLATE_FORMAT_INVALID:
            return "Template has incorrect format or structure";
        case ERROR_TAG_EMPTY:
            return "One or more tags in configuration are empty";
        case ERROR_TAG_INVALID:
            return "Tags contain invalid characters or format";
        case ERROR_TAG_BRACKETS:
            return "Tags must be enclosed in angle brackets < >";
        case ERROR_ESCAPE_INVALID:
            return "Invalid escape sequence in template";
        case ERROR_COMPUTER_TAG_INVALID:
            return "Computer tag contains invalid characters or format";
        case ERROR_COLOR_MISSING:
            return "One or more color fields are missing in configuration";
        case ERROR_COLOR_INVALID:
            return "Color code must be between 0 and 15";
        case ERROR_MEMORY_ALLOCATION:
            return "System memory allocation failed - insufficient resources";
        case ERROR_FRAME_SECTION_EMPTY:
            return "Frame section contains no symbols";
        case ERROR_FRAME_SYMBOL_DUPLICATE:
            return "Same symbol appears in multiple frame sections";
        case ERROR_FRAME_SECTIONS_LIMIT:
            return "Maximum of 9 frame sections allowed";
        case ERROR_FRAME_COLOR_MISSING:
            return "Frame section has no color defined";
        case ERROR_FRAME_INDEX_MISMATCH:
            return "Frame sections must be in order 1,2,3...";
        case ERROR_FRAME_DUPLICATE_INDEX:
            return "Duplicate frame section index detected";
        case ERROR_UNKNOWN:
        default:
            return "An unexpected error occurred in the prompt system";
    }
}

const char* get_error_description(ErrorCode code) {
    switch (code) {
        case ERROR_USERNAME_NOT_FOUND:
            return "Check system user database and environment variables";
        case ERROR_CONFIG_PERMISSION_DENIED:
            return "Verify file permissions and ownership of ~/.prompt_config";
        case ERROR_CONFIG_READ_FAILED:
            return "Ensure config file exists and is not locked by another process";
        case ERROR_CONFIG_PARSE_FAILED:
            return "Review config file syntax and remove any invalid entries";
        case ERROR_FRAME_SYMBOLS_INVALID:
            return "Frame must contain at least 1 symbol separated by semicolons";
        case ERROR_FRAME_SYMBOLS_CONTENT:
            return "Frame symbols must be valid printable characters";
        case ERROR_FRAME_TEMPLATE_MISMATCH:
            return "Frame symbols must correspond to characters used in template\nSee 'FRAME SYMBOLS' section in help (colors help)";
        case ERROR_USER_TAG_MISMATCH:
            return "Ensure 'user:' tag matches the tag used in template string\nSee 'TAGS' section in help (colors help)";
        case ERROR_PATH_TAG_MISMATCH:
            return "Ensure 'path:' tag matches the tag used in template string\nSee 'TAGS' section in help (colors help)";
        case ERROR_SYMBOL_TAG_MISMATCH:
            return "Ensure 'symbol$:' tag matches the tag used in template string\nSee 'TAGS' section in help (colors help)";
        case ERROR_COMPUTER_TAG_MISMATCH:
            return "Ensure 'computer:' tag matches the tag used in template string\nSee 'TAGS' section in help (colors help)";
        case ERROR_TEMPLATE_INVALID:
            return "Template must be a non-empty string with valid characters\nSee 'CONFIGURATION SYNTAX' in help (colors help)";
        case ERROR_TEMPLATE_FORMAT_INVALID:
            return "Template should start with valid frame character and have correct structure\nSee 'EXAMPLES' section in help (colors help)";
        case ERROR_TAG_EMPTY:
            return "User, path and symbol tags must not be empty in configuration\nSee 'TAGS' section in help (colors help)";
        case ERROR_TAG_INVALID:
            return "Tags should contain only alphanumeric characters and underscores\nSee 'TAGS' section in help (colors help)";
        case ERROR_TAG_BRACKETS:
            return "Tags must be formatted as <tag_name> with angle brackets\nSee 'TAGS' section in help (colors help)";
        case ERROR_ESCAPE_INVALID:
            return "Only \\n escape sequences are allowed in template\nSee 'ESCAPE SEQUENCES' in help (colors help)";
        case ERROR_COMPUTER_TAG_INVALID:
            return "Computer tag must be in format <computer> with only alphanumeric characters, underscores or $\nSee 'TAGS' section in help (colors help)";
        case ERROR_COLOR_MISSING:
            return "All color fields must be explicitly defined in config\nSee 'COLOR CODES' section in help (colors help)";
        case ERROR_COLOR_INVALID:
            return "Color codes must be between 0 and 15. Valid values: 0(black),1(red),2(green),3(yellow),4(blue),5(magenta),6(cyan),7(white),8(bright black),9(bright red),10(bright green),11(bright yellow),12(bright blue),13(bright magenta),14(bright cyan),15(bright white)\nSee 'COLOR CODES' section in help (colors help)";
        case ERROR_MEMORY_ALLOCATION:
            return "Close unnecessary applications and try again";
        case ERROR_FRAME_SECTION_EMPTY:
            return "Each frame section must contain at least one symbol";
        case ERROR_FRAME_SYMBOL_DUPLICATE:
            return "A symbol can only appear in one frame section\nSplit your symbols correctly between frame1, frame2, etc.";
        case ERROR_FRAME_SECTIONS_LIMIT:
            return "You can only define up to 9 frame sections (frame1 through frame9)";
        case ERROR_FRAME_COLOR_MISSING:
            return "Each frame section must have a corresponding colors_frameN entry";
        case ERROR_FRAME_INDEX_MISMATCH:
            return "Frame sections must be defined in order (frame1, frame2, frame3...) without gaps";
        case ERROR_FRAME_DUPLICATE_INDEX:
            return "Each frame section index can only appear once";
        case ERROR_UNKNOWN:
            return "Check system logs and try restarting the terminal\nRun colors help for troubleshooting";
        default:
            return "No additional information available\nRun colors help for general assistance";
    }
}

void print_error(ErrorCode code) {
    if (code == SUCCESS) return;
    
    std::cerr << "\033[1;31merror(" << code << ")\033[0m" << std::endl;
    std::cerr << "\033[31m" << get_error_description(code) << "\033[0m" << std::endl;
    
    if (code != ERROR_MEMORY_ALLOCATION && code != ERROR_UNKNOWN) {
        std::cerr << "\nFor detailed configuration guide, run: \033[1mcolors help\033[0m" << std::endl;
    }
}