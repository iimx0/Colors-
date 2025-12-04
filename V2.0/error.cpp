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
        case ERROR_MEMORY_ALLOCATION:
            return "System memory allocation failed - insufficient resources";
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
            return "Frame must contain exactly 5 symbols separated by semicolons";
        case ERROR_FRAME_SYMBOLS_CONTENT:
            return "Frame symbols must be valid printable characters";
        case ERROR_FRAME_TEMPLATE_MISMATCH:
            return "Frame symbols must correspond to characters used in template (─,┌,└,[,])";
        case ERROR_USER_TAG_MISMATCH:
            return "Ensure 'user:' tag matches the tag used in template string";
        case ERROR_PATH_TAG_MISMATCH:
            return "Ensure 'path:' tag matches the tag used in template string";
        case ERROR_SYMBOL_TAG_MISMATCH:
            return "Ensure 'symbol$:' tag matches the tag used in template string";
        case ERROR_TEMPLATE_INVALID:
            return "Template must be a non-empty string with valid characters";
        case ERROR_TEMPLATE_FORMAT_INVALID:
            return "Template should start with valid frame character and have correct structure";
        case ERROR_TAG_EMPTY:
            return "User, path and symbol tags must not be empty in configuration";
        case ERROR_TAG_INVALID:
            return "Tags should contain only alphanumeric characters and underscores";
        case ERROR_TAG_BRACKETS:
            return "Tags must be formatted as <tag_name> with angle brackets";
        case ERROR_ESCAPE_INVALID:
            return "Only \\n escape sequences are allowed in template";
        case ERROR_MEMORY_ALLOCATION:
            return "Close unnecessary applications and try again";
        case ERROR_UNKNOWN:
            return "Check system logs and try restarting the terminal";
        default:
            return "No additional information available";
    }
}

void print_error(ErrorCode code) {
    if (code == SUCCESS) return;
    
    std::cerr << "\033[1;31merror(" << code << ")\033[0m" << std::endl;
    std::cerr << "\033[31m" << get_error_description(code) << "\033[0m" << std::endl;
}