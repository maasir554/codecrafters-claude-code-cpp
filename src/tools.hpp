#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace ToolDefinitions {
    json getReadTool() {
        std::ifstream tool_desc("./src/tool-definitions/read-tool.json");
        if(!tool_desc.is_open()) {
            std::cerr << "Error: Unable to open tool desc (read)." << std::endl;
        }

        std::stringstream buffer;

        buffer << tool_desc.rdbuf();

        std::string ans = buffer.str();
        tool_desc.close();
        
        return json::parse(ans);
    }

    json getWriteTool() {
        std::ifstream tool_desc("./src/tool-definitions/write-tool.json");
        if(!tool_desc.is_open()) {
            std::cerr << "Error: Unable to open tool desc. (write)" << std::endl;
        }

        std::stringstream buffer;

        buffer << tool_desc.rdbuf();

        std::string ans = buffer.str();
        tool_desc.close();
        
        return json::parse(ans);
    }
}
