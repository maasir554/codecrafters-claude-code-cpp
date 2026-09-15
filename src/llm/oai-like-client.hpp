#pragma once
#include "llm-client.hpp"

class OAILikeClient: public LLMClient {
private:
    LLMConfig config;
public:
    explicit OAILikeClient(const LLMConfig& _config);
    json complete(const json& messages, const json& tools) override;
};
