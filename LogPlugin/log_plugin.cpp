#include "log_plugin.h"
#include <cmath>
#include <stdexcept>

std::string LogPlugin::getName() const {
    return "log";
}

double LogPlugin::execute(const std::vector<double>& args) const {
    if (args.empty()) {
        throw std::invalid_argument("log function requires at least 1 argument");
    }

    if (args.size() == 1) {
        // Натуральный логарифм: log(x)
        if (args[0] <= 0) {
            throw std::invalid_argument("log of non-positive number");
        }
        return std::log(args[0]);
    }
    else if (args.size() == 2) {
        // Логарифм по основанию: log(base, x)
        if (args[0] <= 0 || args[0] == 1) {
            throw std::invalid_argument("log base must be positive and not equal to 1");
        }
        if (args[1] <= 0) {
            throw std::invalid_argument("log of non-positive number");
        }
        return std::log(args[1]) / std::log(args[0]);
    }
    else {
        throw std::invalid_argument("log function requires 1 or 2 arguments");
    }
}

extern "C" __declspec(dllexport) IPlugin * create_plugin() {
    return new LogPlugin();
}