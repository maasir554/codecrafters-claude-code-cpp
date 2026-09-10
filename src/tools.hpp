#pragma once
#include <nlohmann/json.hpp>
#include "tool-definitions.hpp"
using json = nlohmann::json;

namespace ToolDefinitions {
    inline json getReadTool() {
        return json::parse(Embedded::read);
    }

    inline json getWriteTool() {
        return json::parse(Embedded::write);
    }
}
