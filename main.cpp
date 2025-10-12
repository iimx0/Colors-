#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <pwd.h>
#include <string>

std::string get_username() {
    const char* user = std::getenv("USER");
    if (user) return user;
    
    struct passwd* pw = getpwuid(geteuid());
    if (pw && pw->pw_name) return pw->pw_name;
    
    return "user";
}

std::string get_current_dir() {
    char buffer[4096];
    if (getcwd(buffer, sizeof(buffer)) == nullptr) {
        return "?";
    }
    
    std::string path = buffer;
    const char* home = std::getenv("HOME");
    if (home && path.find(home) == 0) {
        path.replace(0, strlen(home), "~");
    }
    
    // Фиксированный лимит 30 символов
    const int max_length = 30;
    
    if (path.length() > max_length) {
        // Обрезаем начало пути, оставляем конец
        path = "..." + path.substr(path.length() - max_length + 3);
    }
    
    return path;
}

int main() {
    std::string username = get_username();
    std::string current_dir = get_current_dir();
    
    // Рамка - синий
    std::cout << "\001\033[34m\002┌─────[";
    // Имя пользователя - голубой
    std::cout << "\001\033[36m\002" << username;
    // Рамка - синий
    std::cout << "\001\033[34m\002]──[";
    // Путь - зеленый
    std::cout << "\001\033[32m\002" << current_dir;
    // Рамка - синий
    std::cout << "\001\033[34m\002]\n";
    // Рамка - синий (└──)
    std::cout << "\001\033[34m\002└──";
    // Символ $ - желтый
    std::cout << "\001\033[33m\002$";
    // СБРОС ЦВЕТА для ввода текста (белый)
    std::cout << "\001\033[0m\002 ";
    
    return 0;
}