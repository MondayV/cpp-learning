#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

// 构建转换规则映射
std::map<std::string, std::string> buildMap(std::ifstream &map_file) {
    std::map<std::string, std::string> trans_map;               // 存储转换规则的映射
    std::string key, value;                                     // 用于存储每一行的键和值
    while (map_file >> key && std::getline(map_file, value)) {
        if (!value.empty()) {
            // 去除前导空格
            value = value.substr(value.find_first_not_of(' ')); // 去除前导空格
            trans_map[key] = value;
        }
    }
    return trans_map;
}

// 单词转换
const std::string &transform(const std::string &word, const std::map<std::string, std::string> &trans_map) {
    auto it = trans_map.find(word);                             // 查找单词是否在映射中
    if (it != trans_map.end()) {                                // 如果找到，返回对应的转换值
        return it->second;
    } else {
        return word;
    }
}

// 管理整个转换过程
void word_transform(std::ifstream &map_file, std::ifstream &input_file, std::ofstream &output_file) {
    auto trans_map = buildMap(map_file);                            // 构建转换规则映射
    std::string line;                                               // 用于存储每一行的内容      
    while (std::getline(input_file, line)) {                        // 逐行读取输入文件
        std::istringstream stream(line);                            // 使用字符串流处理每一行
        std::string word;                                           // 用于存储每个单词    
        bool first = true;                                          // 用于控制输出格式  
        while (stream >> word) {                                    // 逐个读取单词
            if (first)
                first = false;
            else
                output_file << " ";
            output_file << transform(word, trans_map);              // 转换单词并输出
        }
        output_file << std::endl;
    }
}

// main函数示例
int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "用法: " << argv[0] << " <规则文件> <输入文件> <输出文件>" << std::endl;
        return 1;
    }
    
    fs::path rules_path(argv[1]);
    fs::path input_path(argv[2]);
    fs::path output_path(argv[3]);
    
    if (!fs::exists(rules_path)) {
        std::cerr << "错误: 规则文件不存在 - " << rules_path << std::endl;
        return 1;
    }
    
    if (!fs::exists(input_path)) {
        std::cerr << "错误: 输入文件不存在 - " << input_path << std::endl;
        return 1;
    }
    
    std::ifstream map_file(rules_path);
    std::ifstream input_file(input_path);
    std::ofstream output_file(output_path);
    
    if (!map_file.is_open()) {
        std::cerr << "无法打开规则文件: " << rules_path << std::endl;
        return 1;
    }
    
    if (!input_file.is_open()) {
        std::cerr << "无法打开输入文件: " << input_path << std::endl;
        return 1;
    }
    
    if (!output_file.is_open()) {
        std::cerr << "无法创建输出文件: " << output_path << std::endl;
        return 1;
    }
    
    word_transform(map_file, input_file, output_file);
    
    map_file.close();
    input_file.close();
    output_file.close();
    
    std::cout << "转换完成! 结果已保存到: " << output_path << std::endl;
    return 0;
}