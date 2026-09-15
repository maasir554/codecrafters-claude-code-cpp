#pragma once
#include "../base.hpp"
#include "../agent-tools/tool.hpp"

class ToolRegistry {
private:
    std::unordered_map<std::string, Tool*> tools_map;

public:     
    void registerTool(Tool* tool);

    Tool* get(const std::string& name) const;

    json definitions() const;
};
