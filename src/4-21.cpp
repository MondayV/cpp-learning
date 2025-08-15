#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

int main()
{
    vector<int> vInt;
    const int SIZE = 10;
    srand((unsigned)time(NULL));            //  设置随机数种子
    cout << "Initial values of the array: " << endl;
    for (int i = 0; i < SIZE; i++)
    {
        vInt.push_back(rand() % 100);       //  生产100以内的随机数
        cout << vInt[i] << " ";             //  使用下标访问元素
    }
    for (int i = 0; i < SIZE; i++)
    {
        cout << vInt[i] << " ";
    }
    cout << endl;
    for(auto &val : vInt)            //  使用范围for语句访问元素
        val = (val % 2 != 0) ? val * 2 : val; //  将奇数元素值乘2
    for(auto it = vInt.begin(); it != vInt.end(); ++it)
        cout << *it << " ";
    cout << endl;
    return 0;
}