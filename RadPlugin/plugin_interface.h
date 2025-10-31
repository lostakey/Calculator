#pragma once
#include <string>
#include <vector>

class IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual std::string getName() const = 0;
    virtual double execute(const std::vector<double>& args) const = 0;
};

extern "C" __declspec(dllexport) IPlugin * create_plugin();