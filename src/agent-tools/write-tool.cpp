#include "tool.hpp"

class WriteTool: public Tool {
private:
    std::string _name;
    json _definition;

public:
    WriteTool() {
        _name = "Write";
        _definition = json::parse(
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
        return _name;
    }

    std::string definition() {
        return _definition;
    }

    ToolResult execute(json& arguments) {
        ToolResult ans;

        std::string path = arguments["file_path"].get<std::string>();
        std::string content = arguments["content"].get<std::string>();

        std::ofstream output_file(path);

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
