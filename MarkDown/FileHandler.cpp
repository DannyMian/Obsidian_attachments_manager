//
// Created by HSM10 on 2024/10/1.
//
#include "FileHandler.h"
#include <iostream>
#include <regex>

namespace fs = std::filesystem;

std::string remove_all_extensions(const std::string& filename);

std::ifstream FileHandler::Open_file(const std::string& path) {
    std::ifstream file(path);  // 使用传入的路径打开文件
    if (!file.is_open()) {
        std::cerr << "Can't open the file: "<<path << std::endl;
        return {};
    }
    return file; // 返回文件对象
}

std::vector<std::string> FileHandler::find_content(std::ifstream& file) {
    std::vector<std::string> contents;
    std::string line;

    // 正则表达式匹配 ![[CONTENT]]，提取 CONTENT 部分
    std::regex link_regex(R"(\!\[\[(.*?)\]\])");
    std::smatch match;
    int line_number = 0;

    while (std::getline(file, line)) {
        ++line_number;

        // 使用正则表达式查找并提取所有符合条件的内容
        while (std::regex_search(line, match, link_regex)) {
            if (match.size() > 1) {
                contents.push_back(match[1]); // 提取 CONTENT 部分
                //std::cout << "Found content in line " << line_number << ": " << match[1] << std::endl;
            }
            // 继续查找该行的下一个匹配项
            line = match.suffix().str();
        }
    }
    //show_vector(contents);
    return contents;
}

// 创建唯一的目标文件夹，附加编号防止重复
std::string FileHandler::create_unique_directory(const std::string& base_directory) {
    std::string unique_directory = base_directory;
    //int counter = 1;

    //// 检查文件夹是否存在，如果存在则在末尾加上数字
    //while (fs::exists(unique_directory)) {
    //    unique_directory = base_directory + "_" + std::to_string(counter);
    //    counter++;
    //}
    if (!fs::exists(unique_directory)) {
        // 创建该文件夹
        fs::create_directory(unique_directory);
        std::cout << "Created directory: " << unique_directory << std::endl;

    }
    

    return unique_directory;
}

// 递归查找与 CONTENT 匹配的文件并移动到目标文件夹
void FileHandler::find_and_move_matching_files(const std::string& root_directory, const std::vector<std::string>& contents, const std::string& target_directory) {
   
    // 使用递归遍历所有子目录和文件
    for (const auto& entry : fs::recursive_directory_iterator(root_directory)) {

        if (entry.is_regular_file()) {  // 仅处理普通文件
            std::string filename = get_filename_with_extension(entry.path());  // 获取文件名（不包括扩展名，同时消去前面的路径）
            //filename = remove_all_extensions(filename);
            // 查找与 CONTENT 匹配的文件
            for (const auto& content_temp : contents) {
                //std::cout << "contents:" << std::endl << content_temp << std::endl;
                //std::string content = remove_all_extensions(content_temp);
                //std::cout << "filename:" << std::endl << filename << std::endl;
                if (filename == content_temp) {
                    //std::cout << filename << std::endl;
                    

                    //if (filename == "Pasted image 20240914132759.png")std::cout <<std::endl<< "HELLO!" << std::endl << std::endl;

                    fs::path source_path = entry.path();
                    fs::path destination_path = fs::path(target_directory) / entry.path().filename();  // 构建目标文件路径

                    // 移动文件到目标文件夹
                    try {
                        fs::rename(source_path, destination_path);
                        //std::cout << "Moved file: " << source_path.string() << " to " << destination_path.string() << std::endl;
                    }
                    catch (const fs::filesystem_error& e) {
                        std::cerr << "Error moving file: " << e.what() << std::endl;
                    }
                }
                /*else {
                    std::cout << "Didn't find any file matches the name!!!" << std::endl;
                }*/
            }
        }
    }
}


std::vector<std::string> FileHandler::get_markdown_files(const std::string& directory) {
    std::vector<std::string> md_files;

    // 使用 recursive_directory_iterator 递归遍历目录及其子目录
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        // 检查文件是否为 Markdown 文件
        if (entry.is_regular_file() && entry.path().extension() == ".md") {
            md_files.push_back(entry.path().string());
        }
    }

    return md_files;
}



std::string remove_all_extensions(const std::string& filename) {
    std::string stem = filename;
    size_t dot_pos = stem.find_last_of('.');

    // 反复去除扩展名，直到没有后缀名或者点位于文件名的开头
    while (dot_pos != std::string::npos && dot_pos > 0) {
        // 获取后缀名
        std::string extension = stem.substr(dot_pos);

        // 如果找到的扩展名是标准的（如 .md, .txt），则移除
        if (extension.length() > 1 && extension.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.") == std::string::npos) {
            stem = stem.substr(0, dot_pos); // 截断至最后一个点之前
            dot_pos = stem.find_last_of('.'); // 继续查找下一个点
        }
        else {
            // 如果点号可能是文件名的一部分（如时间戳），则不再继续移除
            break;
        }
    }

    return stem;
}

// vector

void show_vector(std::vector<std::string> vec) {
    for (auto vec_thing : vec) {
        std::cout << vec_thing << std::endl;
    }
}

std::string get_filename_with_extension(const fs::path& file_path) {
    std::string filename_with_extension = file_path.filename().string(); // 获取完整文件名
    std::string extension = file_path.extension().string(); // 获取扩展名
    std::string stem = file_path.stem().string(); // 获取不带扩展名的文件名

    // 如果需要，可以在这里执行额外的逻辑
    return stem + extension; // 组合返回
}