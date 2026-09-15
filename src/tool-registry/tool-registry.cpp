#include "tool-registry.hpp"

void ToolRegistry::registerTool(Tool* tool){
    tools_map[tool->name()] = tool;
};

Tool* ToolRegistry::get(const std::string& name) const {
    if(!tools_map.count(name)){
        std::cerr << "Tool not registered: " << name;
        throw std::domain_error("Tool not registereed: " + name);
    }

    return tools_map.at(name);
};

json ToolRegistry::definitions() const {
    json ans = json::array();
    
    for(auto [name, tool]: tools_map) {
        ans.push_back(tool->definition());
    }

    return ans;
};
