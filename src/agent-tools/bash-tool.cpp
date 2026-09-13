#include "base.hpp"
#include "tool.hpp"

class BashTool: public Tool {
private:
    std::string _name;
    json _definition;
    
public:
    virtual ~BashTool() {
        _definition = json::parse(
            R"tool_json({
                "type": "function",
                "function": {
                    "name": "Bash",
                    "description": "Execute a shell command",
                    "parameters": {
                        "type": "object",
                        "required": ["command"],
                        "properties": {
                            "command": {
                                "type": "string",
                                "description": "The command to execute"
                            }
                        }
                    }
                }
            })tool_json"
        );
        
        _name = _definition.get<std::string>(); // from the def itself.
    };

    std::string name() const override {
        return _name;
    }

    json definition() const override {
        return _definition;
    }

    ToolResult execute(json& arguments) override {
        ToolResult ans;
        
        std::string command = arguments["command"].get<std::string>();
        std::string wrapped_cmd = "(\n" + command + "\n) 2>&1"; // to capture stderr

        // popen -> very basic access to child processes
        // in next iteration, it will be changed to pipe(), fork(), ... flow
        // because we cannot effectively have timeout, and other process control with this.
        FILE* pipe = popen(wrapped_cmd.c_str(), "r");
        if(!pipe) throw std::runtime_error("failed to start shell command");

        std::string response;
        char buffer[4096]; // 4KB of buffer space for chunks

        // loop for reading file in chunks of 4KB (size of buffer), and accumulating it.
        while(std::fgets(buffer, sizeof(buffer), pipe)) {
            response += buffer; // buffer is a null-terminated '\0' C language string.
            //std::string defines operation of string += C language string.
        }
        // the above loop is blocking in nature, code waites for the pipe to return.
        // hence longer running operations are addressed

        int status = pclose(pipe); // status is the returned value of the child process
        int exit_code = -1;

        if(status != -1) {
            if (WIFEXITED(status)) exit_code = WEXITSTATUS(status);
            else if (WIFSIGNALED(status)) {
                exit_code = 128 + WTERMSIG(status);
            }
        }

        ans.success = (exit_code == 0);
        ans.content = 
            "Exit Code: " + std::to_string(exit_code)  + "\n"
            "Response: " + response;

        return ans;
    }
};
