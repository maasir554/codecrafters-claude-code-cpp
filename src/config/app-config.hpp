#pragma once
#include "../base.hpp"

struct LLMConfig {

    std::string base_url;
    std::string api_key;
    std::string model_id;
    int timeout_ms;

    LLMConfig();
};

