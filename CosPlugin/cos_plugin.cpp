#include "cos_plugin.h"
#include <cmath>
#include <stdexcept>

std::string CosPlugin::getName() const {
    return "cos";
}

double CosPlugin::execute(const std::vector<double>& args) const {
    if (args.size() != 1) {
        throw std::invalid_argument("cos function requires exactly 1 argument");
    }
    return std::cos(args[0]);
}

extern "C" __declspec(dllexport) IPlugin * create_plugin() {
    return new CosPlugin();
}