#include <iostream>
#include <string>
#include "pluginmanager.h"

int main() {
    std::cout << "=== Calculator with Plugins ===\n";

    PluginManager pm;
    pm.loadPlugins("./plugins");

    // Простой тест
    IPlugin* testPlugin = pm.getPlugin("test");
    if (testPlugin) {
        std::vector<double> args = { 5.0 };
        double result = testPlugin->execute(args);
        std::cout << "Test plugin result: " << result << "\n";
    }
    else {
        std::cout << "Test plugin not found\n";
    }

    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input == "quit") break;
        std::cout << "You entered: " << input << "\n";
    }

    return 0;
}