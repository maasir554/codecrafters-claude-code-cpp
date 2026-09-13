#pragma once
#include "tool.hpp"

class ReadTool: public Tool {
private:
    std::string _name;
    json _definition;

    void printStart(std::string path);
    
    void printEnd(bool status);

public:
    ReadTool();
    std::string name() const override;
    json definition() const override;
    ToolResult execute(json& arguments) override;
};   

