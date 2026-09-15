#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <cstdlib>
#include<fstream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct ToolResult {
    bool        success;
    std::string content;
};
