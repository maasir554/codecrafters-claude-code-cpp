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
};
