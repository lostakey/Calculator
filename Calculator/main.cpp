#include <iostream>
#include <string>
#include <filesystem>
#include "pluginmanager.h"
#include "calculator.h"

namespace fs = std::filesystem;

std::string findPluginsPath() {
    std::vector<fs::path> possiblePaths = {
        fs::current_path() / "plugins",
        fs::current_path().parent_path() / "plugins",
        fs::current_path() / ".." / "plugins",
    };

    for (const auto& path : possiblePaths) {
        if (fs::exists(path) && fs::is_directory(path)) {
            return path.string();
        }
    }

    fs::create_directory("plugins");
    return "plugins";
}

int main() {
    std::cout << "=== Advanced Calculator with Plugins ===\n";

    PluginManager pm;
    std::string pluginsPath = findPluginsPath();
    std::cout << "Looking for plugins in: " << pluginsPath << "\n";

    pm.loadPlugins(pluginsPath);

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