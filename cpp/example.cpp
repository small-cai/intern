#include <iostream>
#include <thread>
#include <chrono>
#include "json/json.h"
#include "jsoncpp.cpp"
using namespace std;


int main()
{
    Json::Reader Reader;
    Json::Value DevJson;
    const char* value = [{"gender":"female","race/ethnicity":"group B","parental level of education":"bachelor's degree","lunch":"standard","test preparation course":"none","math score":72,"reading score":72,"writing score":74},{"gender":"female","race/ethnicity":"group C","parental level of education":"some college","lunch":"standard","test preparation course":"completed","math score":69,"reading score":90,"writing score":88},{"gender":"female","race/ethnicity":"group B","parental level of education":"master's degree","lunch":"standard","test preparation course":"none","math score":90,"reading score":95,"writing score":93}];
    Reader.parse(value, DevJson);
    int l = DevJson.size();
    cout << l << endl;
    return 0;
}