/*
/ProjectRoot
├── main.cpp        // 主程序入口，负责启动整个工程
├── FileHandler.h   // FileHandler 类的头文件
├── FileHandler.cpp // FileHandler 类的实现文件
├── GUI.h           // GUI 类的头文件
├── GUI.cpp         // GUI 类的实现文件
*/

#include "FileHandler.h"
#include "GUI.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include <locale>
#include <codecvt>

namespace fs = std::filesystem;

std::string remove_all_extensions(const std::string& filename);


int main() {
    FileHandler file_handler;
    GUI gui;
    //    "C:\Programs\CppProjects\Learning"
        // 通过 GUI 获取文件路径
    //    std::string filepath = gui.get_filepath();
    std::string filepath = R"(C:\Programs\CppProjects\Learning)";

    std::vector<std::string> md_filepaths = file_handler.get_markdown_files(filepath);

    // 遍历所有 Markdown 文件
    for (const auto& md_filepath : md_filepaths) {
        //std::cout << md_filepath << std::endl;
        //std::string md_filepath = filepath;
        std::ifstream file = file_handler.Open_file(md_filepath);
            std::vector<std::string> contents = file_handler.find_content(file);
           

            if (!contents.empty()) {

                // 获取文件夹路径，准备移动文件
                std::filesystem::path file_dir = std::filesystem::path(md_filepath).parent_path();
                //std::cout << file_dir.string() << std::endl;

                // 获取 md 文件的文件名，不包括扩展名
                std::string md_filename = std::filesystem::path(md_filepath).stem().string();

                // 创建 "md_filename + _files" 文件夹的路径
                std::string base_target_directory = file_dir.string() + "\\" + md_filename + "_files";

                std::string target_directory = file_handler.create_unique_directory(base_target_directory);

                //file_handler.find_and_move_matching_files(file_dir.string(), contents, target_directory);
                file_handler.find_and_move_matching_files(filepath, contents, target_directory);
               

            }

        
    }
    // 打开文件并进行处理


    return 0;
}


