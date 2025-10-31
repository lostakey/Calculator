#include "deg_plugin.h"
#include <cmath>
#include <stdexcept>

std::string DegPlugin::getName() const {
    return "deg";
}

double DegPlugin::execute(const std::vector<double>& args) const {
    if (args.size() != 1) {
        throw std::invalid_argument("deg function requires exactly 1 argument");
    }
    return args[0] * (180.0 / 3.14159265358979323846);
}

extern "C" __declspec(dllexport) IPlugin * create_plugin() {
    return new DegPlugin();
}