#include "tool.hpp"

class ReadTool: public Tool {
private:
    std::string _name;
    json _definition;

public:
    ReadTool() {
        this->_name = "Read";

        this->_definition = json::parse(
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

    std::string name() const override {
        return _name;
    }

    json definition() const override {
         return _definition;
    }

    ToolResult execute(json& arguments) override {
        ToolResult ans;

        std::string path = arguments["file_path"].get<std::string>();
        
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

        // std::cout << "Read Tool Response: " << response << std::endl;

        return ans;
    };
};   

