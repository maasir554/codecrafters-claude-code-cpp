#include "base.hpp"

#include "agent-tools/all-tools.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    ReadTool read_tool;
    WriteTool write_tool;
    BashTool bash_tool;

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
        {"model", "gemini-3.5-flash-lite"},
        
        {"messages", json::array({
            {
                {"role", "user"}, 
                {"content", prompt}
            }
        })},

        {"tools", json::array({
            read_tool.definition(),
            write_tool.definition(),
            bash_tool.definition()
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
        std::cerr << "Details: " << response.text << std::endl;
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
        
        if(result["choices"][0]["message"].contains("content") && 
        result["choices"][0]["message"]["content"].is_string()) {
            std::cout <<  result["choices"][0]["message"]["content"].get<std::string>();
        }
        
        for(auto tool_call: result["choices"][0]["message"]["tool_calls"]){
            std::string tool_name = tool_call["function"]["name"].get<std::string>();
            json tool_args = json::parse(tool_call["function"]["arguments"].get<std::string>());
            ToolResult tr;
            tr.content = "Unhandeled Tool";
            tr.success = false;

            if( tool_name == "Read") {
                tr = read_tool.execute(tool_args);
                if(!tr.success) std::cerr << "TOOL FAIL: read\n"; 
            }
            else if(tool_name == "Write") {
                tr = write_tool.execute(tool_args);
                if(!tr.success) std::cout << "TOOL FAIL: Write\n";
            }
            else if(tool_name == "Bash") {
                tr = bash_tool.execute(tool_args);
                if(!tr.success) std::cout<<"Bash command failed\n";
            }
            else {
                std::cerr << "Un-handeled tool: ";
                std::cerr << tool_call["function"]["name"].get<std::string>();
                std::cerr << std::endl;
                tr.content = "Unknown tool called.";
            }

            request_body["messages"].push_back(
                json({
                    {"role", "tool"},
                    {"tool_call_id", tool_call["id"]},
                    {"content", tr.content}
                })
            );
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
    
    std::cout << result["choices"][0]["message"]["content"].get<std::string>() << "\n";

    return 0;
}
