#include <cstdlib>
#include <iostream>
#include <string>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "tools.hpp"
#include "read-tool.hpp"
#include "write-tool.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if (argc < 3 || std::string(argv[1]) != "-p") {
        std::cerr << "Expected first argument to be '-p'" << std::endl;
        return 1;
    }

    std::string prompt = argv[2];

    if (prompt.empty()) {
        std::cerr << "Prompt must not be empty" << std::endl;
        return 1;
    }

    const char* api_key_env = std::getenv("OPENROUTER_API_KEY");
    const char* base_url_env = std::getenv("OPENROUTER_BASE_URL");

    std::string api_key = api_key_env ? api_key_env : "";
    std::string base_url = base_url_env ? base_url_env : "https://openrouter.ai/api/v1";

    if (api_key.empty()) {
        std::cerr << "OPENROUTER_API_KEY is not set" << std::endl;
        return 1;
    }

    json request_body = {
        {"model", "anthropic/claude-haiku-4.5"},
        
        {"messages", json::array({
            {
                {"role", "user"}, 
                {"content", prompt}
            }
        })},

        {"tools", json::array({
            ToolDefinitions::getReadTool(),
            ToolDefinitions::getWriteTool()
        })}
    };

    cpr::Response response = cpr::Post(
        cpr::Url{base_url + "/chat/completions"},
        cpr::Header{
            {"Authorization", "Bearer " + api_key},
            {"Content-Type", "application/json"}
        },
        cpr::Body{request_body.dump()}
    );

    if (response.status_code != 200) {
        std::cerr << "HTTP error: " << response.status_code << std::endl;
        return 1;
    }

    json result = json::parse(response.text);

    if (!result.contains("choices") || result["choices"].empty()) {
        std::cerr << "No choices in response" << std::endl;
        return 1;
    }

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cerr << "Logs from your program will appear here!" << std::endl;

    // execute tools before printing messages.
    // agent loop:

    while(result["choices"][0]["message"]["tool_calls"].size()) {     
        request_body["messages"].push_back(
            result["choices"][0]["message"]
        );

        for(auto tool_call: result["choices"][0]["message"]["tool_calls"]){
            std::string tool_name = tool_call["function"]["name"].get<std::string>();
            
            if( tool_name == "Read") {
                
                json tool_arg = json::parse(tool_call["function"]["arguments"].get<std::string>());
                std::string path = tool_arg["file_path"].get<std::string>();
                
                request_body["messages"].push_back(json({
                    {"role", "tool"},
                    {"tool_call_id", tool_call["id"]},
                    {"content", readToolUtils::getFileText(path)}
                }));
            }

            else if(tool_name == "Write") {
                std::string args_txt = tool_call["function"]["arguments"].get<std::string>();
                json tool_args = json::parse(args_txt);
                std::string path = tool_args["file_path"];
                std::string content = tool_args["content"];
                writeToFile(path, content);

                request_body["messages"].push_back(
                    json({
                        {"role", "tool"},
                        {"tool_call_id", tool_call["id"]},
                        {"content", "content successfully written."}
                    })
                );
            }
    
            else {
                std::cerr << "Un-handeled tool: ";
                std::cerr << tool_call["function"]["name"];
                break;
            }
        }
        
        cpr::Response toolResponse = cpr::Post(
            cpr::Url{base_url + "/chat/completions"},
            cpr::Header{
                {"Authorization", "Bearer " + api_key},
                {"Content-Type", "application/json"}
            },
            cpr::Body{request_body.dump()}
        );

        result = json::parse(toolResponse.text);
    }
    
    std::cout << result["choices"][0]["message"]["content"].get<std::string>();

    return 0;
}
