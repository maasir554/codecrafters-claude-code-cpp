#pragma once
#include "../base.hpp"
#include "../config/app-config.hpp"

class LLMClient {
public:
    virtual ~LLMClient() = default;
    virtual json complete(
        const json& messages,
        const json& tools
    ) = 0;
};
