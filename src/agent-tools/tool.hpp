#pragma once
#include "../base.hpp"

/**
* @interface Tool
* @brief for tool definitions and execution.
* Using command pattern here.
*/

class Tool {
public:
    virtual ~Tool() = default;

    virtual std::string name() const = 0;
    virtual json definition() const = 0;
    virtual ToolResult execute(json& arguments) = 0;
};


