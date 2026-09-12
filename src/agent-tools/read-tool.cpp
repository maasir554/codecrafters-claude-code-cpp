#include "base.hpp"
#include "tool.hpp"

class ReadTool: public Tool {
private:
    std::string tool_name;
    json tool_definition;

public:
    ReadTool() {
        this->tool_name = "Read";

        this->tool_definition = json::parse(
            R"tool_json({
                "type": "function",
                "function": {
                    "name": "Read",
                    "description": "Read and return the contents of a file",
                    "parameters": {
                    "type": "object",
                    "properties": {
                        "file_path": {
                        "type": "string",
                        "description": "The path to the file to read"
                        }
                    },
                    "required": ["file_path"]
                    }
                }
            })tool_json"
        );
    }

    std::string name() {
        return tool_name;
    }

    json definition(){
         return tool_definition;
    }

    ToolResult execute(json& arguments) {
        ToolResult ans;

        std::string path = arguments["file_path"];
        std::ifstream file_buff(path);
        if(!file_buff.is_open()) {
            std::cerr << "Unable to open file at path:\n";
            std::cerr << path << std::endl;
            
            ans.success = false;
            ans.content = "";
        }

        std::stringstream buffer;
        buffer << file_buff.rdbuf();

        std::string response = buffer.str();
        file_buff.close();

        ans.success = true;
        ans.content = response;

        return ans;
    };
};   

