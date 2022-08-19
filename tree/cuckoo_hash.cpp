/*

每个测试用例的输入以3个正整数n1 n2 m开始，其中n1和n2是表T1和T2的大小（n1，n2≤1000和n1 6 = n2），
m是插入的数量。接下来是m个整数值，它是要插入表中的值。所有这些值都是非负的。每个表最初是空的，
表Ti使用散列函数fi（x）= x mod ni。包含3个零的行将终止输入。

*/
#include<bits/stdc++.h>
#include <unordered_map>
#include "cuckoo.h"

#define INSERT_NUM 100000
#define READ_WRITE_RATIO 1000

int main() {
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  CuckooHash<uint32_t, uint32_t> hash(128);
  for (int i = 0; i < INSERT_NUM; i++) {
    Pair<uint32_t, uint32_t> p(i, i);
    hash.insert(p);
  }
  auto durationms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
  std::cout <<"Insert into cuckoo hash takes: " << durationms << "ms" << std::endl;

  for (int i = 0; i < INSERT_NUM; i++) {
    auto p = hash.find(i);
    if (p->first_ != p->second_)
      std::cout << "Data corrup, key:" << p->first_ << " value:" << p->second_ << std::endl;
  }

  start = std::chrono::steady_clock::now();
  std::unordered_map<uint32_t, uint32_t> uhash;
  for (int i = 0; i < INSERT_NUM; i++) {
    uhash[i] = i;
  }
  durationms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
  std::cout <<"Insert into unordered_map takes: " << durationms << "ms" << std::endl;

  start = std::chrono::steady_clock::now();
  for (int k = 0; k < READ_WRITE_RATIO; k++) {
    for (int i = 0; i < INSERT_NUM; i++) {
      hash.find(i);
    }
  }
  durationms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
  std::cout <<"Find in cuckoo hash takes: " << durationms << "ms" << std::endl;

  start = std::chrono::steady_clock::now();
  for (int k = 0; k < READ_WRITE_RATIO; k++) {
    for (int i = 0; i < INSERT_NUM; i++) {
      uhash.find(i);
    }
  }
  durationms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
  std::cout <<"Find in unordered_map takes: " << durationms << "ms" << std::endl;
}
/*
5 7 4
8 18 29 4
6 7 4
8 18 29 4
1000 999 2
1000
2000
0 0 0
*/


/**
*! 杜鹃散列是一种散列形式，它使用两个散列表T1和T2，每个散列表都有自己的散列函数f1（x）和f2（x）。
*? 插入值x的过程如下：首先尝试使用f1（x）将x存储在T1中。如果该位置是空的，那么只需将x存储并完成。
*?否则会发生必须处理的碰撞。设y是该位置当前的值。在T1中用x替换y，然后尝试使用f2（y）将y存储在T2中。
*@param 再说一遍，如果这个位置是空的，你就在那里存储，你就完成了。否则，用y替换那里的值（称为z），然后尝试使用f1（z）
*? 将z存回T1中，依此类推。这个过程继续，在两张桌子之间来回跳动，直到你找到一个空的位置，或者直到有一定数量的掉期发生，
*? 此时你重新组合了这两张桌子（再次与你的教员顾问讨论）。出于这个问题的目的，后一种情况永远不会发生，
*TODO 即，该过程应该一直持续到找到一个空位置，这将保证每个插入的值发生。
*TODO 鉴于两个表的大小和一系列插入，您的工作是确定每个表中存储的内容。
*? （对于那些有兴趣的人来说，杜鹃哈希的名字取决于杜鹃鸟的行为，该杜鹃鸟已知其他鸟类的巢穴，
*? 并将其自己的卵与已经存在的鸡蛋一起放入其中。当较大的杜鹃小鸡孵化时，它会推动其他小鸡从巢中出来，从而得到所有的食物。可怕但有效。）

**/