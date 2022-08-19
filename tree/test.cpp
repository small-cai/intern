#include <iostream>
using namespace std;
class Test
{
private:
    int x, y;

public:

    void setX(int a)
    {
        x = a;
    }
    void setY(int b)
    {
        y = b;
    }
    void getXY(int &px, int &py) //引用
    {
        px = x; //提取x,y值
        py = y;
    }
};
int main()
{
    Test p1, p2;
    p1.setX(1);
    p1.setY(9);
    int a, b;
    p1.getXY(a, b); //将 a=x, b=y
    cout << a << '\t' << b << endl;
    return 0;
}
