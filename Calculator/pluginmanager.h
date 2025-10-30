#pragma once
#include "iplugin.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <windows.h>

class PluginManager {
public:
    PluginManager();
    ~PluginManager();
    void loadPlugins(const std::string& directory);
    IPlugin* getPlugin(const std::string& functionName) const;

private:
    std::unordered_map<std::string, std::unique_ptr<IPlugin>> m_plugins;
    std::vector<HMODULE> m_loadedLibraries;
    void unloadAll();
};