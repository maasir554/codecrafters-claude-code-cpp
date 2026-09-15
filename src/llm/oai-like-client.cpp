#include "oai-like-client.hpp"

OAILikeClient::OAILikeClient(const LLMConfig& _config) {
    OAILikeClient::config = _config;
}

json OAILikeClient::complete(const json& messages, const json& tools) {
    
    json request_body = {
        {"model", config.model_id},
        {"messages", messages},
        {"tools", tools}
    };

    cpr::Response response = cpr::Post(
        cpr::Url{config.base_url + "/chat/completions"},
        cpr::Header{
            {"Authorization", "Bearer " + config.api_key},
            {"Content-Type", "application/json"}
        },
        cpr::Body{request_body.dump()} // messages json to string
    );

    if (response.status_code != 200) {
        std::cerr << "HTTP error: " << response.status_code << std::endl;
        std::cerr << "Details: " << response.text << std::endl;
        throw std::runtime_error("Model request failed: HTTP " + std::to_string(response.status_code));
    }

    json result = json::parse(response.text);

    if (!result.contains("choices") || result["choices"].empty()) {
        std::cerr << "No choices in response" << std::endl;
        throw std::runtime_error("No choices in model response");
    }
    return result.at("choices").at(0).at("message");
}
