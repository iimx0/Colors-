███████████████████████████████████
█─▄▄▄─█─▄▄─█▄─▄███─▄▄─█▄─▄▄▀█─▄▄▄▄█
█─███▀█─██─██─██▀█─██─██─▄─▄█▄▄▄▄─█
▀▄▄▄▄▄▀▄▄▄▄▀▄▄▄▄▄▀▄▄▄▄▀▄▄▀▄▄▀▄▄▄▄▄▀

DESCRIPTION:
  colors generates customizable terminal prompts with dynamic user,
  computer, path, and symbol replacement. Supports Unicode frame characters
  and flexible template configuration.

USAGE:
  colors                    Generate prompt using default/user config
  colors --help             Display this help message
  colors -h                 Short form of --help

CONFIGURATION FILE:
  Location:    ~/.prompt_config
  Auto-create: Created automatically on first run with default config

CONFIGURATION SYNTAX:
  username: template_string
  frame: symbol1;symbol2;symbol3;...
  user: <user_tag>
  computer: <computer_tag>
  path: <path_tag>
  symbol$: <symbol_tag>

TAGS (Replace with actual values):
  <user>      Current username
  <computer>  Current computer hostname
  <path>      Current directory (abbreviated)
  <$>         Prompt symbol ($ for users, # for root)
  Custom tags can be any <tag_name> format

FRAME SYMBOLS:
  Defined in 'frame:' line, separated by semicolons
  Must match characters used in template (bidirectional validation)
  Common symbols: в”Җ (U+2500), в”Ң (U+250C), в”” (U+2514), [, ]

ESCAPE SEQUENCES:
  \n        New line in template
  Only \n is supported, other escapes are invalid

VALIDATION RULES:
  1. Template must contain all frame symbols at least once
  2. All frame-like characters in template must be in frame list
  3. Tags must be enclosed in angle brackets < >
  4. Tags can contain letters, numbers, _, $
  5. Empty tags or invalid characters cause validation errors

COMMENTING:
  # comment                Full line comment
  key: value # comment     Inline comment (not recommended)

DEFAULT CONFIGURATION:
  default: в”Ңв”Җв”Җ[<user>@<computer>]в”Җв”Җ[<path>]\nв””в”Җв”Җ<$>
  frame: в”Җ;в”Ң;в””;[;]
  user: <user>
  computer: <computer>
  path: <path>
  symbol$: <$>

EXAMPLES:

  Example 1 - Basic configuration:
    username: в”Ңв”Җв”Җ[<user>@<computer>]в”Җв”Җ[<path>]\nв””в”Җв”Җ<$>
    frame: в”Җ;в”Ң;в””;[;]
    user: <user>
    computer: <computer>
    path: <path>
    symbol$: <$>

  Example 2 - Custom symbols:
    username: ===[<user>@<computer>]===[<path>]\n==><lol>
    frame: =;[;];>;l;o
    user: <user>
    computer: <computer>
    path: <path>
    symbol$: <lol>

  Example 3 - Disabled field (commented):
    username: в”Ңв”Җв”Җ[<user>@<computer>]в”Җв”Җ[<path>]\nв””в”Җв”Җ<$>
    frame: в”Җ;в”Ң;в””;[;]
    # user: <user>          # Tag <user> won't be replaced
    computer: <computer>
    path: <path>
    symbol$: <$>

ERROR CODES:
  See error messages for specific error codes and descriptions
  Common errors include frame/template mismatch, invalid tags,
  escape sequence errors, and configuration parsing failures.

AUTOMATIC USER CREATION:
  When a new user runs colors, their configuration is automatically
  created based on the 'default' configuration if it doesn't exist.

CACHE SYSTEM:
  Configurations are cached for performance. Cache is invalidated
  when configuration file is modified. Use reload if needed.

DEVELOPMENT:
  Tool is designed for extensibility. Future features may include
  color support, additional dynamic variables, and plugin system.

SEE ALSO:
  Configuration validation occurs automatically. For debugging,
  check ~/.prompt_config syntax and ensure frame symbols match.
