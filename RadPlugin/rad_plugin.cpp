#include "rad_plugin.h"
#include <cmath>
#include <stdexcept>

std::string RadPlugin::getName() const {
    return "rad";
}

double RadPlugin::execute(const std::vector<double>& args) const {
    if (args.size() != 1) {
        throw std::invalid_argument("rad function requires exactly 1 argument");
    }
    return args[0] * (3.14159265358979323846 / 180.0);
}

extern "C" __declspec(dllexport) IPlugin * create_plugin() {
    return new RadPlugin();
}