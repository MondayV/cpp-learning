/*
编写程序实现文本查询，接受一个文件，并与用户交互来查询单词。
使用vector、map和set容器保存来自文件的数据并生产查询结果
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cctype>
#include <cstdlib>
#include "../include/make_plural.h"

using namespace std;

using line_no = vector<string>::size_type;
vector<string> file;
map<string, set<line_no>> wm;

string cleanup_str(const string &word)
{
    string ret;
    for (auto it = word.begin(); it != word.end(); ++it) {
        if (!ispunct(*it)) 
            ret += tolower(*it);
    }
    return ret;
}

void input_text(ifstream &is)
{
    string text;
    while (getline(is, text)) 
    {
        file.push_back(text);
        int n = file.size() - 1; 
        istringstream line(text);
        string word;
        while (line >> word){
                wm[cleanup_str(word)].insert(n);
            }
    }
    
}

ostream &query_and_print(const string &sought, ostream & os)
{
    auto loc = wm.find(sought);
    if(loc == wm.end()){
        os << sought << "出现了0次" << endl;
    } else {
        auto &lines = loc->second;
        os << sought << "出现了" << lines.size() 
            << make_plural(lines.size(), "次", "次") << endl;
        for (auto num : lines)
            os << "\t(第" << num + 1 << "行) " << *(file.begin() + num) << endl;
    }
    return os;
}

void runQueries(ifstream &infile)
{
    input_text(infile);
    while(true) {
        cout << "enter word to look for, or q to quit: ";
        string s;
        if(!(cin >> s) || s == "q") break;
        query_and_print(s, cout) << endl;
    }
}

int main(int argc, char **argv)
{
    ifstream infile;
    if(argc < 2 || !(infile.open(argv[1]), infile)) {
        cerr << "无法打开文件"  << endl;
        return EXIT_FAILURE;
    }
        runQueries(infile);
    return EXIT_SUCCESS;
}