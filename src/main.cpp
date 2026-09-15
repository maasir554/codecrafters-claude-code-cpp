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
        
        LLMConfig config; // data about model endpoint, key, read from environment variables.
        
        OAILikeClient client(config); // make client from the config.
        
        // declare available tools.
        ReadTool read_tool;
        WriteTool write_tool;
        BashTool bash_tool;
        
        ToolRegistry tools; // make a centeralized tool registry.

        // register the tools in the registy.
        tools.registerTool(&read_tool);
        tools.registerTool(&write_tool);
        tools.registerTool(&bash_tool);

        //system prompt
        std::string system_prompt = 
        "You are a you are a helpful general purpose and coding agent. "
        "Before each group of tool calls, briefly explain what you "
        "are about to do and why in one short sentence. "
        "Use ordinary assistant text for this explanation. "
        "After receiving tool results, continue working until the "
        "request is complete. Do not claim success before checking "
        "the tool results.";

        // initilize the agent with client, and tool registry. 
        Agent agent(client, tools, 50, system_prompt); // max requests to LLM: 50
        std::cout << agent.run(argv[2]) << '\n';
    } 
    catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
