#pragma once
#include "plugin_interface.h"

class DegPlugin : public IPlugin {
public:
    std::string getName() const override;
    double execute(const std::vector<double>& args) const override;
};