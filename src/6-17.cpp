#include <iostream>
#include <string>
using namespace std;

bool HasUpper(const string &str) {  // 检查字符串中是否有大写字母
    for (auto c : str) 
        if (isupper(c)) 
            return true;
    return false;
}

void ChangeToLower(string &str) {   // 将字符串中的大写字母转换为小写字母
    for (auto &c : str) 
        c = tolower(c);
}

int main() {
    
    cout << "Enter a string: ";
    string str;
    cin >> str;
    if (HasUpper(str)) {
        cout << "The string contains uppercase letters." << endl;
        ChangeToLower(str);
        cout << "Converted to lowercase: " << str << endl;
    } else {
        cout << "The string does not contain uppercase letters." << endl;
    }

    return 0;
}
