#include <iostream>
#include <string>

using namespace std;

int main()
{
    string finalgrade;
    int grade;
    cout << "Enter your grade: " << endl;
    while (cin >> grade && grade >= 0 && grade <= 100)
    {
        if(grade > 90)
            finalgrade = "high pass";
        else if(grade > 75)
            finalgrade = "pass";
        else if(grade > 60)
            finalgrade = "low pass";
        else
            finalgrade = "fail";
        cout << "Your final level is: " << finalgrade << endl;
        cout << "Enter the grade you want to check: " << endl;
    }

    return 0;
}