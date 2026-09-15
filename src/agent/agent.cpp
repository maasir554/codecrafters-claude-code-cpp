#include "agent.hpp"
#include <iostream>
#include <stdexcept>

Agent::Agent(LLMClient& client, ToolRegistry& tools, int max_turns, std::string sys_prompt)
    : _client(client), _tools(tools), _max_turns(max_turns), _sys_prompt(sys_prompt) {
    if (max_turns <= 0) throw std::invalid_argument("max_turns must be positive");
}

void Agent::resetConversation() {
    _messages = json::array();
    
    if(!_sys_prompt.empty()) {
        _messages.push_back({
            {"role", "system"},
            {"content", _sys_prompt}
        });
    }
}

std::string Agent::run(const std::string& prompt) {
    if (prompt.empty()) throw std::invalid_argument("Prompt must not be empty");
    _messages.push_back({{"role", "user"}, {"content", prompt}});
    const json definitions = _tools.definitions();

    for (int turn = 0; turn < _max_turns; ++turn) {
        json message = _client.complete(_messages, definitions);
        validateAssistantMessage(message);

        if (!hasToolCalls(message)) {
            std::string answer = finalText(message);
            _messages.push_back(message);
            return answer;
        }

        _messages.push_back(message);
        logAssistantCommentary(message);
        for (const auto& call : message.at("tool_calls")) {
            ToolResult result = executeToolCall(call);
            appendToolResult(call.at("id").get<std::string>(), result);
        }
    }
    throw std::runtime_error("Agent reached its maximum model turns");
}

void Agent::validateAssistantMessage(const json& message) const {
    if (!message.is_object() || message.value("role", std::string{}) != "assistant") {
        throw std::runtime_error("Expected an assistant message");
    }
    auto calls = message.find("tool_calls");
    if (calls != message.end() && !calls->is_null() && !calls->is_array()) {
        throw std::runtime_error("tool_calls must be an array");
    }
    if (!hasToolCalls(message)) return;

    // Validate every ID before adding a message that requires tool responses.
    for (const auto& call : *calls) {
        if (!call.contains("id") || !call["id"].is_string()) {
            throw std::runtime_error("Tool call has no string ID");
        }
    }
}

bool Agent::hasToolCalls(const json& message) const {
    auto calls = message.find("tool_calls");
    return calls != message.end() && calls->is_array() && !calls->empty();
}

ToolResult Agent::executeToolCall(const json& call) {
    try {
        const auto& function = call.at("function");
        const auto name = function.at("name").get<std::string>();
        json arguments = json::parse(function.at("arguments").get<std::string>());
        Tool* tool = _tools.get(name);
        
        if (!tool) throw std::runtime_error("Registered tool is null");
        return tool->execute(arguments);
    } 
    catch (const std::exception& error) {
        return {false, std::string("Tool error: ") + error.what()};
    }
}

void Agent::appendToolResult(const std::string& id, const ToolResult& result) {
    _messages.push_back({
        {"role", "tool"}, 
        {"tool_call_id", id}, 
        {"content", result.content}
    });
}

std::string Agent::finalText(const json& message) const {
    if (!message.contains("content") || !message["content"].is_string()) {
        throw std::runtime_error("Final assistant message has no text content");
    }
    return message["content"].get<std::string>();
}

void Agent::logAssistantCommentary(const json& message) const {
    if (message.contains("content") && message["content"].is_string()) {
        std::cerr << message["content"].get<std::string>() << '\n';
    }
}
