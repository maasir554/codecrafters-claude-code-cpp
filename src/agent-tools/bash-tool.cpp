#include "bash-tool.hpp"
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <sys/wait.h>

void BashTool::printStart(std::string cmd) {

    std::cout << "==[Tool]: Bash Tool =============\n";
    std::cout << "$ " << cmd << "\n";
    std::cout << "-------------------------------\n";
}
void BashTool::printEnd(std::string ans) {
    std::cout << "Result\n";
    std::cout << ans << "\n";
    std::cout << "________________________________\n";
}

BashTool::BashTool() {
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
    
    _name = _definition["function"]["name"].get<std::string>(); // from the def itself.
};

std::string BashTool::name() const {
    return _name;
}

json BashTool::definition() const {
    return _definition;
}

ToolResult BashTool::execute(json& arguments) {
    ToolResult ans;
    
    std::string command = arguments["command"].get<std::string>();
    
    printStart(command);
    
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
        "Response: " + response == "" ? "(no output)" : response;

    printEnd(ans.content);
    return ans;
}
