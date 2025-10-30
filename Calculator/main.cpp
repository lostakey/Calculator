#include <iostream>
#include <string>
#include "pluginmanager.h"

int main() {
    std::cout << "Calculator Starting...\n";

    PluginManager pm;
    pm.loadPlugins("./plugins");

    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input == "quit") break;
        std::cout << "You entered: " << input << "\n";
    }

    return 0;
}