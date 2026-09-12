#pragma once
#include "base.hpp"

using json = nlohmann::json;

void writeToFile(std::string path, std::string content) {
    std::ofstream output_file(path);

    if(!output_file.is_open()) {
        std::cerr << "Error in writing to the file.";
        return;
    }

    output_file << content;

    output_file.close();
}
