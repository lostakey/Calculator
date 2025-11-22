#include "pluginmanager.h"
#include <iostream>
#include <windows.h>
#include <filesystem>

namespace fs = std::filesystem;

PluginManager::PluginManager() = default;

PluginManager::~PluginManager() {
    unloadAll();
}

void PluginManager::loadPlugins(const std::string& directory) {
    std::cout << "Loading plugins from: " << directory << "\n";

    if (!fs::exists(directory)) {
        std::cout << "Plugins directory doesn't exist. Creating...\n";
        fs::create_directory(directory);
        return;
    }

    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".dll") {
            std::string dllPath = entry.path().string();
            std::cout << "Found DLL: " << dllPath << "\n";

            HMODULE hModule = LoadLibraryA(dllPath.c_str());
            if (!hModule) {
                std::cout << "Failed to load: " << dllPath << "\n";
                continue;
            }

            auto createFunc = (IPlugin * (*)())GetProcAddress(hModule, "create_plugin");
            if (!createFunc) {
                std::cout << "No create_plugin function in: " << dllPath << "\n";
                FreeLibrary(hModule);
                continue;
            }

            try {
                std::unique_ptr<IPlugin> plugin(createFunc());
                std::string pluginName = plugin->getName();

                if (m_plugins.find(pluginName) != m_plugins.end()) {
                    std::cout << "Warning: Function '" << pluginName << "' already exists. Skipping.\n";
                    FreeLibrary(hModule);
                }
                else {
                    m_plugins[pluginName] = std::move(plugin);
                    m_loadedLibraries.push_back(hModule);
                    std::cout << "Loaded: " << pluginName << "\n";
                }

            }
            catch (const std::exception& e) {
                std::cout << "Error creating plugin: " << e.what() << "\n";
                FreeLibrary(hModule);
            }
        }
    }

    std::cout << "Total plugins loaded: " << m_plugins.size() << "\n";
}

IPlugin* PluginManager::getPlugin(const std::string& functionName) const {
    auto it = m_plugins.find(functionName);
    if (it != m_plugins.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<std::string> PluginManager::getAvailableFunctions() const {
    std::vector<std::string> names;
    for (const auto& pair : m_plugins) {
        names.push_back(pair.first);
    }
    return names;
}

void PluginManager::unloadAll() {
    m_plugins.clear();
    for (HMODULE handle : m_loadedLibraries) {
        FreeLibrary(handle);
    }
    m_loadedLibraries.clear();
}