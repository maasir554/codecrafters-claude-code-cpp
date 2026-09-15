#include "agent/agent.hpp"
#include "agent-tools/all-tools.hpp"
#include "config/app-config.hpp"
#include "llm/oai-like-client.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    try {
        if (argc < 3 || std::string(argv[1]) != "-p") {
            std::cerr << "Expected first argument to be '-p'\n";
            return 1;
        }
        LLMConfig config;
        OAILikeClient client(config);
        ReadTool read_tool;
        WriteTool write_tool;
        BashTool bash_tool;
        ToolRegistry tools;
        tools.registerTool(&read_tool);
        tools.registerTool(&write_tool);
        tools.registerTool(&bash_tool);
        Agent agent(client, tools, 50);
        std::cout << agent.run(argv[2]) << '\n';
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
