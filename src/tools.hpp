#include <nlohmann/json.hpp>
using json = nlohmann::json;

json readTool = {
    {"type", "function"},
    {
        "function", {
            {"name", "Read"},
            {"description", "Read and return the contents of a file"},
            {
                "parameters", {
                    {"type", "object"},
                    {
                        "properties", {
                            {
                                "file_path", {
                                    {"type", "string"},
                                    {"description", "the path to the file to read"}
                                }
                            }
                        }
                    }
                } 
            },
            {"required", json::array({"file_path"})}
        } 
    }
};