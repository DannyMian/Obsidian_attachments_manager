//
// Created by HSM10 on 2024/10/1.
//
#include "FileHandler.h"
#include <iostream>
#include <regex>

namespace fs = std::filesystem;

std::string remove_all_extensions(const std::string& filename);

std::ifstream FileHandler::Open_file(const std::string& path) {
    std::ifstream file(path);  // ʹ�ô����·�����ļ�
    if (!file.is_open()) {
        std::cerr << "Can't open the file: "<<path << std::endl;
        return {};
    }
    return file; // �����ļ�����
}

std::vector<std::string> FileHandler::find_content(std::ifstream& file) {
    std::vector<std::string> contents;
    std::string line;

    // ������ʽƥ�� ![[CONTENT]]����ȡ CONTENT ����
    std::regex link_regex(R"(\!\[\[(.*?)\]\])");
    std::smatch match;
    int line_number = 0;

    while (std::getline(file, line)) {
        ++line_number;

        // ʹ��������ʽ���Ҳ���ȡ���з�������������
        while (std::regex_search(line, match, link_regex)) {
            if (match.size() > 1) {
                contents.push_back(match[1]); // ��ȡ CONTENT ����
                //std::cout << "Found content in line " << line_number << ": " << match[1] << std::endl;
            }
            // �������Ҹ��е���һ��ƥ����
            line = match.suffix().str();
        }
    }
    //show_vector(contents);
    return contents;
}

// ����Ψһ��Ŀ���ļ��У����ӱ�ŷ�ֹ�ظ�
std::string FileHandler::create_unique_directory(const std::string& base_directory) {
    std::string unique_directory = base_directory;
    //int counter = 1;

    //// ����ļ����Ƿ���ڣ������������ĩβ��������
    //while (fs::exists(unique_directory)) {
    //    unique_directory = base_directory + "_" + std::to_string(counter);
    //    counter++;
    //}
    if (!fs::exists(unique_directory)) {
        // �������ļ���
        fs::create_directory(unique_directory);
        std::cout << "Created directory: " << unique_directory << std::endl;

    }
    

    return unique_directory;
}

// �ݹ������ CONTENT ƥ����ļ����ƶ���Ŀ���ļ���
void FileHandler::find_and_move_matching_files(const std::string& root_directory, const std::vector<std::string>& contents, const std::string& target_directory) {
   
    // ʹ�õݹ����������Ŀ¼���ļ�
    for (const auto& entry : fs::recursive_directory_iterator(root_directory)) {

        if (entry.is_regular_file()) {  // ��������ͨ�ļ�
            std::string filename = get_filename_with_extension(entry.path());  // ��ȡ�ļ�������������չ����ͬʱ��ȥǰ���·����
            //filename = remove_all_extensions(filename);
            // ������ CONTENT ƥ����ļ�
            for (const auto& content_temp : contents) {
                //std::cout << "contents:" << std::endl << content_temp << std::endl;
                //std::string content = remove_all_extensions(content_temp);
                //std::cout << "filename:" << std::endl << filename << std::endl;
                if (filename == content_temp) {
                    //std::cout << filename << std::endl;
                    

                    //if (filename == "Pasted image 20240914132759.png")std::cout <<std::endl<< "HELLO!" << std::endl << std::endl;

                    fs::path source_path = entry.path();
                    fs::path destination_path = fs::path(target_directory) / entry.path().filename();  // ����Ŀ���ļ�·��

                    // �ƶ��ļ���Ŀ���ļ���
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

    // ʹ�� recursive_directory_iterator �ݹ����Ŀ¼������Ŀ¼
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        // ����ļ��Ƿ�Ϊ Markdown �ļ�
        if (entry.is_regular_file() && entry.path().extension() == ".md") {
            md_files.push_back(entry.path().string());
        }
    }

    return md_files;
}



std::string remove_all_extensions(const std::string& filename) {
    std::string stem = filename;
    size_t dot_pos = stem.find_last_of('.');

    // ����ȥ����չ����ֱ��û�к�׺�����ߵ�λ���ļ����Ŀ�ͷ
    while (dot_pos != std::string::npos && dot_pos > 0) {
        // ��ȡ��׺��
        std::string extension = stem.substr(dot_pos);

        // ����ҵ�����չ���Ǳ�׼�ģ��� .md, .txt�������Ƴ�
        if (extension.length() > 1 && extension.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.") == std::string::npos) {
            stem = stem.substr(0, dot_pos); // �ض������һ����֮ǰ
            dot_pos = stem.find_last_of('.'); // ����������һ����
        }
        else {
            // �����ſ������ļ�����һ���֣���ʱ����������ټ����Ƴ�
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
    std::string filename_with_extension = file_path.filename().string(); // ��ȡ�����ļ���
    std::string extension = file_path.extension().string(); // ��ȡ��չ��
    std::string stem = file_path.stem().string(); // ��ȡ������չ�����ļ���

    // �����Ҫ������������ִ�ж�����߼�
    return stem + extension; // ��Ϸ���
}