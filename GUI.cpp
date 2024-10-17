//
// Created by HSM10 on 2024/10/1.
//
#include "GUI.h"
#include <iostream>

std::string GUI::get_filepath() {
    std::string filepath;
    std::cout << "=== File Path Input ===" << std::endl;
    std::cout << "Please enter the file path: ";
    std::getline(std::cin, filepath);
    std::cout << "You entered: " << filepath << std::endl;
    return filepath;
}

void GUI::display_message(const std::string& message) {
    std::cout << message << std::endl;
}
