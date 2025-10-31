#include "pow_plugin.h"
#include <cmath>
#include <stdexcept>

std::string PowPlugin::getName() const {
    return "pow";
}

double PowPlugin::execute(const std::vector<double>& args) const {
    if (args.size() != 2) {
        throw std::invalid_argument("pow function requires exactly 2 arguments");
    }
    return std::pow(args[0], args[1]);
}

extern "C" __declspec(dllexport) IPlugin * create_plugin() {
    return new PowPlugin();
}