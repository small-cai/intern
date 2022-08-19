#include <bits/stdc++.h>
using namespace std;

class Quote
{
public:
    Quote() = default;
    Quote(const string &book, double sale_price) : bookNo(book), price(sale_price) {}
    string isbn() const
    {
        return bookNo;
    }
    virtual double net_price(size_t n) const
    {
        return n * price;
    } // 方便派生类各自定义自己的版本

    virtual ~Quote() = default; //基类通常需要定义一个虚析构函数，即使不执行任何操作也是如此

private:
    string bookNo;

protected:
    double price = 0.0;
};

class Bulk_quote : public Quote
{
public:
    Bulk_quote() = default;
    double net_price(size_t) const override; // 允许派生类显式地注明他将使用哪个成员函数改写基类的虚函数
    Bulk_quote(const string &, double, size_t, double);

private:
    size_t min_qty = 0;
    double discount = 0.0;
};

double print(ostream &os, const Quote &item, size_t n)
{
    // 根据传入的item形参的对象类型，调用Quote::net_price()
    double ret = item.net_price(n);
   os << "ISBN: " << item.isbn() << "# sold: " << n << "total due: " << ret << endl;
    return ret;
}

int main()
{
    Quote basic("name_one",50);

    print(cout, basic, 20);
    
    Bulk_quote derived("name_two",50,5,0.19);

    // print(cout,derived,10);

    // basic = derived;3
   
    // cout << basic.net_price(20) << endl;

    return 0;
}