#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include <thread> // 执行多线程
#include <mutex>
#include <future>
#include <chrono>
//#include <emscripten/emscripten.h>
#include "json/json.h"
#include "jsoncpp.cpp"

#ifndef EM_PORT_API
#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#if defined(__cplusplus)
#define EM_PORT_API(rettype) extern "C" rettype EMSCRIPTEN_KEEPALIVE
#else
#define EM_PORT_API(rettype) rettype EMSCRIPTEN_KEEPALIVE
#endif
#else
#if defined(__cplusplus)
#define EM_PORT_API(rettype) extern "C" rettype
#else
#define EM_PORT_API(rettype) rettype
#endif
#endif
#endif

typedef unsigned int ul;

// template<class Type>
// std::ostream &operator << (std::ostream &out, std::vector<Type> &val){
//   for(auto it:val){
//     out << it;
//   }
//   return out;
// }

/*
 * @param 获取CPU核数。
 */
ul getCpuNum()
{
  ul nCpu = std::max(std::thread::hardware_concurrency(), (unsigned int)1) - 1; // 核数 -1
  return nCpu;
}

std::mutex m;
void MutilProcess(const Json::Value &value, ul index1, ul index2,
                  std::map<std::vector<std::string>, std::unordered_map<std::string, int>> &Map,
                  std::map<std::vector<std::string>, std::unordered_map<std::string, int>> &Map_Cnt,
                  std::string measures[], std::string dimensions[], ul ld, ul lm)
{
  // std::lock_guard<std::mutex> lk(m);
  for (ul i = index1; i < index2; i++)
  {
    std::vector<std::string> ARR(ld);

    for (ul j = 0; j < ld; j++)
    {
      ARR[j] = value[i][dimensions[j]].asString();
    }
    for (ul k = 0; k < lm; k++)
    {
      Map[ARR][measures[k]] += value[i][measures[k]].asInt();
      Map_Cnt[ARR][measures[k]] += 1;
    }
  }
}

//
std::string aggregate(const Json::Value value, std::string dimensions[],
                      std::string measures[], std::string ops[], ul dl, ul ml, ul ol)
{
  // init
  ul ld = dl;
  ul lm = ml;
  ul lo = ol;
  ul rl = value.size();

  //定义hash表
  std::map<std::vector<std::string>, std::unordered_map<std::string, int>> Map;
  Map.clear();
  std::map<std::vector<std::string>, std::unordered_map<std::string, int>> Map_Cnt;
  Map_Cnt.clear();

  ul sum = 0;
  /*
   * 预处理数据切分
   */
  ul CPU_CORE = getCpuNum();
  ul L_SIZE = rl;

  ul M_TIMES = rl / CPU_CORE;
  ul RES_TIMES = rl % CPU_CORE;

  ul index1, index2;
  // std::thread Process[CPU_CORE + 1];
  std::future<void> M[CPU_CORE + 1];

  for (ul i = 0; i < CPU_CORE; i++)
  {
    if (i == 0)
    {
      index1 = 0;
      index2 = index1 + M_TIMES;
    }
    else
    {
      index1 = index2;
      index2 = index1 + M_TIMES;
    }

    // MutilProcess(value, index1, index2, Map, Map_Cnt, measures, dimensions, ld, lm);
    //  thread 常用处理
    // Process[i] = std::thread(MutilProcess, std::ref(value), index1, index2, std::ref(Map), std::ref(Map_Cnt), std::ref(measures), std::ref(dimensions), ld, lm);
    M[i] = std::async(std::launch::async, MutilProcess, std::ref(value), index1, index2,
                      std::ref(Map), std::ref(Map_Cnt), std::ref(measures), std::ref(dimensions), ld, lm);
  }
  if (RES_TIMES)
  {
    index1 = index2;
    index2 = L_SIZE;
    M[CPU_CORE] = std::async(std::launch::async, MutilProcess, std::ref(value), index1, index2,
                             std::ref(Map), std::ref(Map_Cnt), std::ref(measures), std::ref(dimensions), ld, lm);
  }
  ul Process_Num = (RES_TIMES > 0) ? CPU_CORE + 1 : CPU_CORE;

  for (ul j = 0; j < Process_Num; j++)
  {
    // Process[j].join();
    M[j].get();
  }

  Json::Value root;
  Json::FastWriter writer;
  Json::Value person;
  Json::Value cj;
  Json::Value ccj;
  for (ul i = 0; i < lo; i++)
  {
    // root.clear();
    if (ops[i] == "sum")
    {
      //std::cout << "sum" << std::endl;
      ccj.clear();
      for (auto it : Map)
      {
        for (ul k = 0; k < lm; k++)
        {
          root.clear();
          // cj.clear();
          //root.append(measures[k]);
          for (auto itt : it.first)
          {
            //std::cout << itt << "<->";
            root.append(itt);
          }
          double q = Map[it.first][measures[k]] * 1.0;
          root.append(q);
          cj[measures[k]].append(root);
          // std::cout << measures[k] << " " << std::setprecision(10)
          //           << Map[it.first][measures[k]] * 1.0 << std::endl;
       
        }
      }
      person[ops[i]] = cj;
    }
    else if (ops[i] == "mean")
    {
      //std::cout << "mean" << std::endl;
      ccj.clear();
      for (auto it : Map)
      {
        for (ul k = 0; k < lm; k++)
        {
          root.clear();
          // cj.clear();
          //root.append(measures[k]);
          for (auto itt : it.first)
          {
            //std::cout << itt << "<->";
            root.append(itt);
          }
          double q = Map[it.first][measures[k]] * 1.0 / Map_Cnt[it.first][measures[k]];
          root.append(q);
          cj[measures[k]].append(root);
          // ccj.append(root);
          // std::cout << measures[k] << " " << std::setprecision(10) << (Map[it.first][measures[k]] * 1.0 / Map_Cnt[it.first][measures[k]]) << std::endl;
        }
      }
      person[ops[i]] = cj;
    }
  }
  // 转json为字符串
  return person.toStyledString();
  /*
  * 想办法转为字符串 (已完成)
  TODO 转为更简洁的，key<->value 形式
  */
 
  // return " ";
}

int main()
{
  // std::ios::sync_with_stdio(false);
  //   cout <<"sign"<<endl;
  Json::Reader reader;
  Json::Value root;

  //从文件夹中读数据
  std::ifstream in("ds-students-service.json", std::ios::binary);

  if (!in.is_open())
  {
    std::cerr << "Error opening file\n";
    return 1;
  }
  bool check = reader.parse(in, root);
  if (check)
  {
    // std::cout << root << std::endl;
    Json::Value data = root["dataSource"];

    // std::cout << data.size() << std::endl;
    std::string dimensions[] = {"gender", "lunch"};
    ul dl = sizeof(dimensions) / sizeof(dimensions[0]);
    std::string measures[] = {"math score", "reading score"};
    ul ml = sizeof(measures) / sizeof(measures[0]);
    std::string ops[] = {"mean", "sum"};
    ul ol = sizeof(ops) / sizeof(ops[0]);
    auto start = std::chrono::high_resolution_clock::now(); //开始时间
    std::cout << aggregate(data, dimensions, measures, ops, dl, ml, ol)<<std::endl;
    auto end = std::chrono::high_resolution_clock::now(); //结束时间
    __int64 duration = (end - start).count();
    std::cout << "程序运行时间：" << std::setprecision(10)
              << duration / 1000000.0 << "ms"
              << std::endl;
  }
  else
  {
    std::cerr << "Not a json file\n";
  }

  return 0;
}
