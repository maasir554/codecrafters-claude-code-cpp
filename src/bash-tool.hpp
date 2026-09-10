#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
#include<fstream>


namespace bashTool {
    
    struct BashResponse {
        int exit_code;
        std::string output;
        BashResponse(int exit_code, std::string output):
        exit_code(exit_code), output(output) {};
    };

    BashResponse executeCommand(std::string command) {
        std::string wrapped = "(\n" + command + "\n) 2>&1";

        FILE* pipe = popen(wrapped.c_str(), "r");

        if(!pipe) throw std::runtime_error("failed bash tool: pipe did not open");

        std::string output;
        char buffer[4096];

        while(std::fgets(buffer, sizeof(buffer), pipe)) {
            output += buffer;
        }

        int status = pclose(pipe);
        int exit_code = -1;

        if(status != -1) {
            if(WIFEXITED(status)) exit_code = WEXITSTATUS(status);
            else if(WIFSIGNALED(status)) exit_code = 128 + WTERMSIG(status);
        }

        return BashResponse(exit_code, output);
    }
}

