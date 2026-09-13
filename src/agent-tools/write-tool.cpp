#include "tool.hpp"

class WriteTool: public Tool {
private:
    std::string _name;
    json _definition;

public:
    WriteTool() {
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
                        },
                    }
                }
            })tool_json"
        );
        
        _name = _definition.get<std::string>();
    }

    std::string name() const override {
        return _name;
    }

    json definition() const override {
        return _definition;
    }

    ToolResult execute(json& arguments) override {
        ToolResult ans;

        std::string path = arguments["file_path"].get<std::string>();
        std::string content = arguments["content"].get<std::string>();

        std::ofstream output_file(path); // note: prev data will be erased at time of opening pre-exisiting a file.

        if(!output_file.is_open()) {
            std::cerr << "Unable to open the file to write\n";
            std::cerr << "Path: " << path << std::endl;
            ans.success = false;
            ans.content = "file cannot be opened.";
            return ans;
        }

        output_file << content;
        
        output_file.close();
        
        ans.success = true;
        ans.content = "write successful.";
        return ans;
    }
};
