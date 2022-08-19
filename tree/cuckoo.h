template<class K, class V>
struct Pair {
  Pair(const K& k = K(), const V& v = V()) : first_(k), second_(v) {}
  K first_;
  V second_;
};

template<class K>
uint32_t hash1(const K& k);
template<class K>
uint32_t hash2(const K& k);

template<>
uint32_t hash1<uint32_t>(const uint32_t& k) {
  return k;
}

template<>
uint32_t hash2<uint32_t>(const uint32_t& k) {
  uint32_t a = k;
  a = (a+0x7ed55d16) + (a<<12);
  a = (a^0xc761c23c) ^ (a>>19);
  a = (a+0x165667b1) + (a<<5);
  a = (a+0xd3a2646c) ^ (a<<9);
  a = (a+0xfd7046c5) + (a<<3);
  a = (a^0xb55a4f09) ^ (a>>16);
  return a;
}

template<class K, class V>
class CuckooHash
{
public:
  explicit CuckooHash(uint32_t originSz) {
    sz_ = roundUpPowerOf2(originSz | 2); // make sure size is at least 2
    szMask_ = sz_ - 1;
    table1_.resize(sz_);
    table2_.resize(sz_);
    nullBits1_.resize(sz_, true);
    nullBits2_.resize(sz_, true);
  }

  // Must be in one of table is exist
  Pair<K, V>* find(const K& k) {
    uint32_t p1 = getPos1(k);
    if (!nullBits1_[p1] && table1_[p1].first_ == k)
      return &table1_[p1];

    uint32_t p2 = getPos2(k);
    if (!nullBits2_[p2] && table2_[p2].first_ == k)
      return &table2_[p2];

    return nullptr;
  }

  bool erase(const K& k) {
    uint32_t p1 = getPos1(k);
    if (!nullBits1_[p1] && table1_[p1].first_ == k) {
      nullBits1_[p1] = true;
      return true;
    }

    uint32_t p2 = getPos2(k);
    if (!nullBits2_[p2] && table2_[p2].first_ == k) {
      nullBits2_[p2] = true;
      return true;
    }

    return false;
  }

  void insert(const Pair<K, V>& pair) {
retry:
    uint32_t firstTryPos = getPos1(pair.first_);
    Pair<K, V> curPair = pair;
    uint32_t curPos = firstTryPos;
    bool toTbl1 = true;

    if (nullBits1_[firstTryPos]) {
      std::swap(table1_[firstTryPos], curPair);
      nullBits1_[firstTryPos] = false;
      return;
    } else {
      std::swap(table1_[curPos], curPair);
      toTbl1 = false;
    }

    while (true) {
      if (toTbl1) {
        curPos = getPos1(curPair.first_);
        // If same key to be inserted into table1 same position, break a circle
        if (curPos == firstTryPos && curPair.first_ == pair.first_)
          break;
        if (nullBits1_[curPos]) {
          std::swap(table1_[curPos], curPair);
          nullBits1_[curPos] = false;
          return;
        } else {
          std::swap(table1_[curPos], curPair);
          toTbl1 = false;
        }
      } else {
        curPos = getPos2(curPair.first_);
        if (nullBits2_[curPos]) {
          std::swap(table2_[curPos], curPair);
          nullBits2_[curPos] = false;
          return;
        } else {
          std::swap(table2_[curPos], curPair);
          toTbl1 = true;
        }
      }
    }
    reHash();
    goto retry;
  }

  void debugString() {
    std::cout << "=== Cuckoo hash ===\n";
    for (int i = 0; i < sz_; i++) {
      std::cout << (nullBits1_[i] ? "-" : std::to_string(table1_[i].first_)) << "\t" <<
                (nullBits2_[i] ? "-" : std::to_string(table2_[i].first_)) << std::endl;
    }
  }


private:
  inline uint32_t getPos1(const K& k) {
    return hash1<K>(k) & szMask_;
  }

  inline uint32_t getPos2(const K& k) {
    return hash2<K>(k) & szMask_;
  }

  unsigned int roundUpPowerOf2(uint32_t v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
  }

  // new a cuckoo hash Map, and insert one by one
  // recessive rehashes might be necessary before this succeeds.
  void reHash() {
    CuckooHash<K, V> newHash(sz_ * 2);
    for (int i = 0; i < sz_; i++) {
      if (!nullBits1_[i])
        newHash.insert(table1_[i]);
      if (!nullBits2_[i])
        newHash.insert(table2_[i]);
    }
    this->swap(newHash);
  }

  void swap(const CuckooHash<K, V>& hash) {
    table1_ = hash.table1_;
    table2_ = hash.table2_;
    nullBits1_ = hash.nullBits1_;
    nullBits2_ = hash.nullBits2_;
    sz_ = hash.sz_;
    szMask_ = hash.szMask_;
  }

private:
  std::vector<Pair<K, V>> table1_;
  // todo(): how to implicit a empty bucket more efficiently
  std::vector<bool> nullBits1_;
  std::vector<Pair<K, V>> table2_;
  std::vector<bool> nullBits2_;
  uint32_t sz_;
  uint32_t szMask_;
};