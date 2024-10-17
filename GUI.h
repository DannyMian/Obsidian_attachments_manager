#pragma once
//
// Created by HSM10 on 2024/10/1.
//

#ifndef GUI_H
#define GUI_H

#include <string>

class GUI {
public:
    std::string get_filepath();
    void display_message(const std::string& message);
};

#endif // GUI_H

