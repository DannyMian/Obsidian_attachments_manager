#pragma once
//
// Created by HSM10 on 2024/10/1.
//

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

class FileHandler {
public:
    std::vector<std::string> get_markdown_files(const std::string& directory);
    std::ifstream Open_file(const std::string& path);
    std::vector<std::string> find_content(std::ifstream& file);
    std::string create_unique_directory(const std::string& base_directory);
    void find_and_move_matching_files(const std::string& root_directory, const std::vector<std::string>& contents, const std::string& target_directory);
};

#endif // FILEHANDLER_H

void show_vector(std::vector<std::string> vec);
std::string get_filename_with_extension(const std::filesystem::path& file_path);
