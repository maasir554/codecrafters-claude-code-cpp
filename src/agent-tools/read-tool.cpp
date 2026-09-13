#include "read-tool.hpp"

// private functions: 

void ReadTool::printStart(std::string path) {
    std::cout << "====[Tool]: Read Tool  ============\n";
    std::cout << "Requested Path: " << path << "\n";
    std::cout << "-----------------------------------\n";
}
    
void ReadTool::printEnd(bool status) {
    std::cout << "Status: " << (status ? "Done" : "Failed") <<"\n";
    std::cout << "_____________________________\n";
}

// public functions:

ReadTool::ReadTool() {
    ReadTool::_definition = json::parse(
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

    ReadTool::_name = _definition["function"]["name"].get<std::string>();
}

std::string ReadTool::name() const {
    return _name;
}

json ReadTool::definition() const {
        return _definition;
}

ToolResult ReadTool::execute(json& arguments) {
    ToolResult ans;

    std::string path = arguments["file_path"].get<std::string>();
    
    ReadTool::printStart(path);

    std::ifstream file_buff(path);
    
    if(!file_buff.is_open()) {
        std::cerr << "Unable to open file at path:\n";
        std::cerr << path << std::endl;
        
        ans.success = false;
        ans.content = "";
    }
    else {
        std::stringstream buffer;
        buffer << file_buff.rdbuf();
        
        std::string response = buffer.str();
        file_buff.close();
        
        ans.success = true;
        ans.content = response;
    }
    
    ReadTool::printEnd(ans.success);
    return ans;
}

