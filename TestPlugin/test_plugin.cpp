#include "test_plugin.h"
#include <cmath>
#include <stdexcept>

std::string TestPlugin::getName() const {
    return "test";
}

double TestPlugin::execute(const std::vector<double>& args) const {
    if (args.empty()) return 0.0;
    // Простая тестовая функция: умножаем первый аргумент на 2
    return args[0] * 2.0;
}

extern "C" __declspec(dllexport) IPlugin * create_plugin() {
    return new TestPlugin();
}