#pragma once
#include "tool.hpp"

class BashTool: public Tool {
private:
    std::string _name;
    json _definition;

    void printStart(std::string cmd);
    void printEnd(std::string ans);

public:
    BashTool();
    std::string name() const override;
    json definition() const override;
    ToolResult execute(json& arguments) override;
};
