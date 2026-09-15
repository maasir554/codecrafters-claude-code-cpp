#pragma once
#include "../base.hpp"
#include "../llm/llm-client.hpp"
#include "../tool-registry/tool-registry.hpp"

class Agent {
public:
    Agent(LLMClient& client, ToolRegistry& tools, int max_turns, std::string sys_prompt);
    std::string run(const std::string& prompt);
    void resetConversation();
    
    private:
    void validateAssistantMessage(const json& message) const;
    bool hasToolCalls(const json& message) const;
    ToolResult executeToolCall(const json& call);
    void appendToolResult(const std::string& id, const ToolResult& result);
    std::string finalText(const json& message) const;
    void logAssistantCommentary(const json& message) const;
    
    LLMClient& _client;
    ToolRegistry& _tools;
    int _max_turns;
    json _messages = json::array();
    std::string _sys_prompt;
};
