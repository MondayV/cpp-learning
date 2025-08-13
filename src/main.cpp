#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <filesystem>

// 构建转换规则映射
std::map<std::string, std::string> buildMap(std::ifstream &map_file) {
    std::map<std::string, std::string> trans_map;
    std::string key, value;
    while (map_file >> key && std::getline(map_file, value)) {
        if (!value.empty()) {
            // 去除前导空格
            value = value.substr(value.find_first_not_of(' '));
            trans_map[key] = value;
        }
    }
    return trans_map;
}

// 单词转换
const std::string &transform(const std::string &word, const std::map<std::string, std::string> &trans_map) {
    auto it = trans_map.find(word);
    if (it != trans_map.end()) {
        return it->second;
    } else {
        return word;
    }
}

// 管理整个转换过程
void word_transform(std::ifstream &map_file, std::ifstream &input_file) {
    auto trans_map = buildMap(map_file);
    std::string line;
    while (std::getline(input_file, line)) {
        std::istringstream stream(line);
        std::string word;
        bool first = true;
        while (stream >> word) {
            if (first)
                first = false;
            else
                std::cout << " ";
            std::cout << transform(word, trans_map);
        }
        std::cout << std::endl;
    }
}

// main函数示例
int main() {
    std::ifstream map_file("F:/c++/cpp-learning/test/word_map.txt");
    std::ifstream input_file("F:/c++/cpp-learning/test/input.txt");
 if (!map_file || !input_file) {
        std::cerr << "文件打开失败！请检查：" << std::endl;
        std::cerr << "1. 文件是否在程序同一目录" << std::endl;
        std::cerr << "2. 文件名是否正确" << std::endl;
        std::cerr << "3. 文件扩展名是否隐藏(如.txt.txt)" << std::endl;
        return 1;
    }

    word_transform(map_file, input_file);
    return 0;
}