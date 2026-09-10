#pragma once
#include<fstream>
#include<iostream>
#include<string>

namespace readToolUtils {
    void printFile(std::string path) {
        std::ifstream file_buff(path);

        if(file_buff.is_open()) {
            std::cout << file_buff.rdbuf();
            file_buff.close();
        }
        else {
            std::cerr << "Error: Unable to open the file." << std::endl;
        }
    }

    std::string getFileText(std::string path) {
        std::ifstream file_buff(path);

        if(!file_buff.is_open()) {
            std::cerr << "Error: Unable to open the file." << std::endl;
        }
        std::stringstream buffer;
        buffer << file_buff.rdbuf();
        std::string ans = buffer.str();
        file_buff.close();
        return ans;

    }
};
