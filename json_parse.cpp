#include <bits/stdc++.h>
using namespace std;
#define INT 0
#define DOU 1
#define STR 2
#define BOOL 3
#define LIST 4
#define DICT 5
class Val
{
public:
    int INT_VAL;
    double DOU_VAL;
    string STR_VAL;
    bool BOOL_VAL;
    int type;
    vector<Val> List;
    map<Val, Val> dict;
    void del()
    {
        if (type == STR)
            STR_VAL.clear();
        if (type == LIST)
            List.clear();
        if (type == DICT)
            dict.clear();
    }
    Val(){};
    ~Val()
    {
        del();
    }
    Val(const int &x)
    {
        del();
        INT_VAL = x;
        type = INT;
    }
    Val(const double &x)
    {
        del();
        DOU_VAL = x;
        type = DOU;
    }
    Val(const string &x)
    {
        del();
        STR_VAL = x;
        type = STR;
    }
    Val(const char *x)
    {
        del();
        type = STR;
        STR_VAL = string(x);
    }
    Val(const bool &x)
    {
        del();
        BOOL_VAL = x;
        type = BOOL;
    }
    Val(initializer_list<Val> l)
    {
        del();
        type = LIST;
        for (Val x : l)
            List.push_back(x);
    }
    void add(Val x)
    {
        if (type == LIST)
        {
            List.push_back(x);
        }
    }
    void put(Val key, Val value)
    {
        if (type == DICT)
            dict[key] = value;
    }
    Val &operator[](Val i)
    {
        if (i.type == LIST)
            return List[i.INT_VAL];
        else
            return dict[i];
    }

    string str();
};
/*
 json 支持类型
 1，数字
 2，字符串
 3，bool
 4,数组
 5，对象
 6，null
*/

bool operator<(const Val &a, const Val &b)
{
    if (a.type != b.type)
        return a.type < b.type;
    else
    {
        if (a.type == INT)
            return a.INT_VAL < b.INT_VAL;
        if (a.type == DOU)
            return a.DOU_VAL < b.DOU_VAL;
        if (a.type == STR)
            return a.STR_VAL < b.STR_VAL;
        if (a.type == LIST)
            return a.List < b.List;
        if (a.type == DICT)
            return a.dict < b.dict;
        return true;
    }
}
ostream &operator<<(ostream &out, const Val &v)
{
    if (v.type == INT)
        out << v.INT_VAL;
    if (v.type == DOU)
        out << v.DOU_VAL;
    if (v.type == STR)
        out << "\"" << v.STR_VAL << "\"";
    if (v.type == BOOL)
    {
        if (v.BOOL_VAL)
        {
            out << "true";
        }
        else
        {
            out << "false";
        }
    }
    if (v.type == LIST)
    {
        out << "[";
        for (int i = 0; i < v.List.size(); i++)
        {
            if (i)
                out << ",";
            out << v.List[i];
        }
        out << "]";
    }
    if (v.type == DICT)
    {
        out << "{";
        for (auto i = v.dict.begin(); i != v.dict.end(); i++)
        {
            if (i != v.dict.begin())
                out << ",";
            out << i->first << ":" << i->second;
        }
        out << "}";
    }
    return out;
}

string Val::str()
{
    stringstream ss;
    ss << (*this);
    return ss.str();
}

stringstream ss;
Val parser_val();
Val parser_num();
Val parser_str();
Val parser_bool();
Val parser_arr();
Val parser_map();

Val parser_num()
{
    string s;
    while (isdigit(ss.peek()) || ss.peek() == 'e' || ss.peek() == '-' || ss.peek() == '+')
        s.push_back(ss.get());
    if (count(s.begin(), s.end(), '.') || count(s.begin(), s.end(), 'e'))
    {
        return stof(s);
    }
    else
    {
        return stoi(s);
    }
}

Val parser_str()
{
    ss.get();
    string s;
    while (ss.peek() != '"')
        s.push_back(ss.get());
    ss.get();
    return Val(s);
}

Val parser_bool()
{
    if (ss.peek() == 'f')
    {
        for(int i = 0; i < 5;i++)
            ss.get();

        return Val(false);
    }
    else
    {
        for(int i = 0; i < 4;i++) // sizeof(true)/sizeof(true[0]) = 4
            ss.get();
        return Val(true);
    }
    return Val(true);
}
Val parser_arr()
{
    ss.get(); // 去掉'['
    Val vec;
    vec.type = LIST;
    while (ss.peek() != ']')
    {
        vec.add(parser_val());
        while (ss.peek() != ']' && (ss.peek() == ' ' || ss.peek() == '\t' || ss.peek() == '\n' || ss.peek() == ','))
            ss.get();
    }
    ss.get();
    return vec;
}

Val parser_map()
{
    ss.get(); // 去掉'{'
    Val dict;
    dict.type = DICT;
    while (ss.peek() != '}')
    {
        Val key = parser_val();
        while (ss.peek() == ' ' || ss.peek() == ':')  ss.get();
        Val val = parser_val();
        dict.put(key, val);
        while (ss.peek() != '}' && (ss.peek() == ' ' || ss.peek() == '\t' || ss.peek() == '\n' || ss.peek() == ','))ss.get();
    }
    ss.get();
    return dict;
}

Val parser_val()
{
    while (ss.peek() != -1)
    {
        if (ss.peek() == ' ' || ss.peek() == '\n' || ss.peek() == '\t')
            ss.get();
        else if (ss.peek() == '"')
        {
            return parser_str();
        }
        else if (ss.peek() == 'f' || ss.peek() == 't')
        {
            return parser_bool();
        }
        else if (ss.peek() == '[')
        {
            return parser_arr();
        }
        else if (ss.peek() == '{')
        {
            return parser_map();
        }
        else
        {
            return parser_num();
        }
    }
    return 0;
}
Val parser(string s)
{
    ss.clear();
    ss << s;
    return parser_val();
}

int main()
{
    // Val x = 100;
    // cout << x << endl
    // Val x;
    // x.type = DICT;
    // x.put("key", "value");
    // cout << x << endl;

    ifstream fin("tem.json");
    stringstream ss;
    ss << fin.rdbuf();
    Val x = parser(ss.str());
    cout << "email"<<x["email"] << endl;
    cout <<"phone" << x["phone"]["home"] << endl;
    // cout << x << endl;
    return 0;
}
