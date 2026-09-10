#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace ToolDefinitions {
    json getReadTool() {
        std::ifstream write_tool("./tool-definitions/read-tool.json");
        if(!write_tool.is_open()) {
            std::cerr << "Error: Unable to open the file." << std::endl;
        }

        std::stringstream buffer;

        buffer << write_tool.rdbuf();

        std::string ans = buffer.str();
        write_tool.close();
        
        return json::parse(ans);
    }

    json getWriteTool() {
        std::ifstream write_tool("./tool-definitions/write-tool.json");
        if(!write_tool.is_open()) {
            std::cerr << "Error: Unable to open the file." << std::endl;
        }

        std::stringstream buffer;

        buffer << write_tool.rdbuf();

        std::string ans = buffer.str();
        write_tool.close();
        
        return json::parse(ans);
    }
}
