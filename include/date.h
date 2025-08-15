#ifndef DATE_H_INCLUDED
#define DATE_H_INCLUDED

#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

class date
{
    public:
        friend ostream& operator<<(ostream &os, const date &d);

        date() = default;
        date(string &ds);

        unsigned y() const { return year; }
        unsigned m() const { return month; }
        unsigned d() const { return day; }
    private:
        unsigned year,month,day;
};

const string month_name[] = 
//  月份名称数组
    {"January","February","March","April","May","June",
    "July","August","September","October","November","December"};

const string month_abbr[] =
//  月份缩写数组
    {"Jan","Feb","Mar","Apr","May","Jun",
    "Jul","Aug","Sep","Oct","Nov","Dec"};

const int days[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

inline int get_month(string &ds, int &end_of_month)
{
    int i,j;
    for(i = 0; i < 12; i++){
        //  检查月份名称
        for(j = 0;j < month_abbr[i].size(); j++)
            if(ds[j] != month_abbr[i][j])   //月份名称不匹配
                break;
            if(j == month_abbr[i].size())   // 月份名称匹配
                break;
    }

    if (i == 12)
        throw invalid_argument("不是合法月份名称");

    if(ds[j] == ' '){
        end_of_month = j + 1;
        return i + 1;
    }

    for(; j < month_name[i].size(); j++)
        if(ds[j] != month_name[i][j])   //月份名称不匹配
            break;

    if(j == month_name[i].size() && ds[j] == ' '){
        end_of_month = j + 1;
        return i + 1;
    }

    throw invalid_argument("不是合法月份名称");
}

inline int get_day(string &ds, int month, int &p)
{
    {
        size_t q;
        int day = stoi(ds.substr(p), &q);
        if (day < 1 || day > days[month - 1])
            throw invalid_argument("不是合法日期");
            p += q;
        return day;
    }
}

inline int get_year(string &ds, int &p)
{
    size_t q;
    int year = stoi(ds.substr(p), &q);
    if (p + q < ds.size())
        throw invalid_argument("不是合法年份");
    return year;
}

date::date(string &ds)
{
    int p;
    size_t q;

    if ((p = ds.find_first_of("0123456789")) == string::npos)
        throw invalid_argument("没有数字，非法日期");

    if (p > 0){
        month = get_month(ds, p);;
        day = get_day(ds, month, p);
        if (ds[p] != ' ' && ds[p] != ',')
            throw invalid_argument("非法间隔符");
        p++;
        year = get_year(ds, p);
    } else {
        month = stoi(ds, &q);
        p = q;
        if (month < 1 || month > 12)
            throw invalid_argument("不是合法月份");
        if(ds[p++] != '/')
            throw invalid_argument("非法间隔符");
        day = get_day(ds, month, p);
        if(ds[p++] != '/')
            throw invalid_argument("非法间隔符");
        year = get_year(ds, p);
    }
}

ostream & operator<<(ostream& out, const date& d)
{
    out << d.y() << "年" << d.m() << "月" << d.d() << "日" << endl;
    return out;
}
#endif // DATE_H_INCLUDED