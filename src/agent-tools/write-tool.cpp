#include "write-tool.hpp"
#include <fstream>
#include <iostream>

void WriteTool::printStart(std::string path, int size) {
    std::cout << "====[Tool]: Write Tool ===========\n";
    std::cout << "Requested Path: " << path << "\n";
    std::cout << "Number of chars: " << size << "\n";
    std::cout << "-----------------------------------\n";
}

void WriteTool::printEnd(bool status) {
    std::cout << "Status: " << (status ? "Done" : "Failed") << "\n";
    std::cout << "__________________________________\n";
}

WriteTool::WriteTool() {
    _definition = json::parse(
        R"tool_json({
            "type": "function",
            "function": {
                "name": "Write",
                "description": "Write content to a file",
                "parameters": {
                    "type": "object",
                    "required": ["file_path","content"],
                    "properties": {
                        "file_path": {
                            "type": "string",
                            "description": "The path to the file to write to"
                        },
                        "content": {
                            "type": "string",
                            "description": "The content to write to the file"
                        }
                    }
                }
            }
        })tool_json"
    );
    
    _name = _definition["function"]["name"].get<std::string>();
}

std::string WriteTool::name() const {
    return _name;
}

json WriteTool::definition() const {
    return _definition;
}

ToolResult WriteTool::execute(json& arguments) {
    ToolResult ans;

    std::string path = arguments["file_path"].get<std::string>();
    std::string content = arguments["content"].get<std::string>();

    this->printStart(path, content.size());

    std::ofstream output_file(path); // note: prev data will be erased at time of opening pre-exisiting a file.

    if(!output_file.is_open()) {
        std::cerr << "Unable to open the file to write\n";
        std::cerr << "Path: " << path << std::endl;
        ans.success = false;
        ans.content = "file cannot be opened.";
        this->printEnd(false);
        return ans;
    }

    output_file << content;
    
    output_file.close();
    
    ans.success = true;
    ans.content = "write successful.";
    this->printEnd(true);
    return ans;
}
