#include "test_plugin.h"
#include <cmath>

std::string TestPlugin::getName() const {
    return "test";
}

double TestPlugin::execute(const std::vector<double>& args) const {
    if (args.empty()) return 0.0;
    return args[0] * 2.0; // Просто умножаем на 2 для теста
}

extern "C" __declspec(dllexport) IPlugin * create_plugin() {
    return new TestPlugin();
}