#include "pluginmanager.h"
#include <iostream>
#include <windows.h>

PluginManager::PluginManager() = default;

PluginManager::~PluginManager() {
    unloadAll();
}

void PluginManager::loadPlugins(const std::string& directory) {
    std::cout << "Loading plugins from: " << directory << "\n";
    // Заглушка
}

IPlugin* PluginManager::getPlugin(const std::string& functionName) const {
    auto it = m_plugins.find(functionName);
    if (it != m_plugins.end()) {
        return it->second.get();
    }
    return nullptr;
}

void PluginManager::unloadAll() {
    m_plugins.clear();
    for (HMODULE handle : m_loadedLibraries) {
        FreeLibrary(handle);
    }
    m_loadedLibraries.clear();
}