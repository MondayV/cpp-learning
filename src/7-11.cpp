#include <iostream>
#include <string>

class Sales_data
{
    public:
        Sales_data() = default;
        Sales_data(const std::string &book) : bookNo(book) {  }
        Sales_data(const std::string &book, const unsigned num,
            const double sellp, const double salep);
        Sales_data(std::istream &is) ;
        //声明输入运算符为友元函数
        friend std::istream& operator>>(std::istream &is, Sales_data &item)
        {
            double price = 0.0;
            is >> item.bookNo >> item.units_sold >> item.sellingprice >> price;
            if (is) {
                item.saleprice = price;
                if (item.sellingprice != 0.0)
                    item.discount = (item.saleprice - item.sellingprice) / item.sellingprice;
            } else {
                item = Sales_data();
            }
            return is;
        }
    public:
            std::string bookNo;
            unsigned units_sold = 0;
            double sellingprice = 0.0;
            double saleprice = 0.0;
            double discount = 0.0;
};

Sales_data::Sales_data(const std::string &book, const unsigned num,
    const double sellp, const double salep)
{
    bookNo = book;
    units_sold = num;
    sellingprice = sellp;
    saleprice = salep;
    if(sellingprice != 0)
        discount = saleprice / sellingprice;
}

Sales_data::Sales_data(std::istream &is)
{
    is >> *this;
}