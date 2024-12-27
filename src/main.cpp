#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <print>

#include <unistd.h>
#include <pwd.h>

#include "commands.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::print("{}: Necesitamos al menos 2 argumentos para funcionar.\n"
                   "Use /help para más información.\n",
                   argv[0]);
        return -1;
    }

    if (geteuid() != 0 && std::strcmp(argv[1], "/help") != 0) {
        auto uid = geteuid();
        auto* pw = getpwuid(uid);
        if (pw) {
            std::print("{}: Necesitamos permisos de usuario root para funcionar.\n"
                       "El usuario actual es {}.\n",
                       argv[0], pw->pw_name);
        } else {
            std::print("{}: No hemos podido obtener información sobre el usuario {}.\n",
                       argv[0], uid);
        }
        return -1;
    }

    std::map<std::string, int(*)(std::vector<std::string>)> commands;
    commands["/help"] = help;
    commands["/restore"] = restore;
    commands["/search"] = search;

    if (commands.find(argv[1]) == commands.end()) {
        std::print("{}: El argumento {} no fue encontrado\n"
                   "Use /help para más información.\n",
                   argv[0], argv[1]);
        return -1;
    }

    std::vector<std::string> args;
    args.assign(argv, argv + argc);
    return commands[argv[1]](args);
}
