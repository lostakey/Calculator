#include <iostream>
#include <string>
#include "pluginmanager.h"
#include "calculator.h"

int main() {
    std::cout << "=== Advanced Calculator with Plugins ===\n";

    PluginManager pm;
    pm.loadPlugins("./plugins");

    Calculator calc(&pm);

    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "quit" || input == "exit") break;
        if (input.empty()) continue;

        try {
            double result = calc.calculate(input);
            std::cout << "= " << result << "\n";
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}