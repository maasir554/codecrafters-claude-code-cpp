#pragma once
#include "tool.hpp"

class WriteTool: public Tool {
private:
    std::string _name;
    json _definition;

    void printStart(std::string path, int size);
    void printEnd(bool status);

public:
    WriteTool();
    std::string name() const override;
    json definition() const override;
    ToolResult execute(json& arguments) override;
};
