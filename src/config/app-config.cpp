#include "app-config.hpp"

LLMConfig::LLMConfig() {
    // Blaze Code will be the name of this coding agent.
    // set the following variables in the shell environment
    // use $ export BLAZECODE_API_KEY=...
    
    const char* api_key_env = std::getenv("BLAZECODE_API_KEY");
    const char* base_url_env = std::getenv("BLAZECODE_BASE_URL");
    const char* model_id_env = std::getenv("BLAZECODE_MODEL_ID");

    if (!api_key_env) api_key_env = std::getenv("OPENROUTER_API_KEY");
    if (!base_url_env) base_url_env = std::getenv("OPENROUTER_BASE_URL");

    this->base_url = base_url_env ? base_url_env : "https://openrouter.ai/api/v1";
    this->api_key = api_key_env ? api_key_env : "";
    this->model_id = model_id_env ? model_id_env : "gemini-3.5-flash-lite";

    if (api_key.empty()) throw std::runtime_error("BLAZECODE_API_KEY (or OPENROUTER_API_KEY) is not set");
}
