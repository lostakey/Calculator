#include "sin_plugin.h"
#include <cmath>
#include <stdexcept>

std::string SinPlugin::getName() const {
    return "sin";
}

double SinPlugin::execute(const std::vector<double>& args) const {
    if (args.size() != 1) {
        throw std::invalid_argument("sin function requires exactly 1 argument");
    }
    return std::sin(args[0]);
}

extern "C" __declspec(dllexport) IPlugin * create_plugin() {
    return new SinPlugin();
}