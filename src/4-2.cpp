#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

int main()
{
    vector<int> vec;
    srand((unsigned)time(NULL));
    cout << "Enter 10 integers: " << endl;
    for (int i = 0; i < 10; ++i) 
        vec.push_back(rand() % 100); 
    cout << "static was:" << endl;
    for(auto c: vec)
        cout << c << " ";
    cout << endl;
    cout << "Verify whether the added parentheses are correct: " << endl;
    cout << "vec.begin()value: " << *vec.begin() << endl;
    cout << "(vec.begin())value: " << *(vec.begin()) << endl;
    cout << "vec.begin() + 1 value: " << *(vec.begin() + 1) << endl;
    cout << "(*(vec.begin())) + 1 value: " << (*(vec.begin())) + 1 << endl;

    return 0;
}