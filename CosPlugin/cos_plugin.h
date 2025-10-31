#pragma once
#include "plugin_interface.h"

class CosPlugin : public IPlugin {
public:
    std::string getName() const override;
    double execute(const std::vector<double>& args) const override;
};