/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef NIT_UTIL_COMBINATIONS_H_
#define NIT_UTIL_COMBINATIONS_H_

#include <string>

namespace nit {

/**
 * Generates the set of all N choose K combinations of K
 * indices less than N.
 */
class combinations {
 public:
  combinations(size_t n, size_t k) : n_(n), k_(k), didnull_(true) {
    if (k_ > 0)
      comb_ = new size_t[k_];
    else
      comb_ = nullptr;
    for (size_t i = 0; i < k_; ++i)
      comb_[i] = i;
  }

  void reset(size_t n, size_t k) {
    if (k > k_) {
      delete[] comb_;
      comb_ = new size_t[k];
    }
    k_ = k;
    n_ = n;
    for (size_t i = 0; i < k_; ++i)
      comb_[i] = i;
  }

  combinations(const combinations& c)
      : n_(c.n_), k_(c.k_), didnull_(c.didnull_) {
    if (k_ > 0)
      comb_ = new size_t[k_];
    else
      comb_ = nullptr;
    for (size_t i = 0; i < k_; ++i)
      comb_[i] = c.comb_[i];
  }

  void reset() {
    for (size_t i = 0; i < k_; ++i)
      comb_[i] = i;
    didnull_ = true;
  }

  std::string str() const {
    std::string ret;
    for (size_t i = 0; i < size(); i++)
      ret += std::to_string((*this)[i]) + " ";
    return ret;
  }

  uint64_t getMask() const {
    uint64_t mask = 0;
    size_t* pcom = comb_;
    for (size_t i = 0; i < size(); i++)
      mask |= static_cast<uint64_t>(0x01) << *pcom++;
    return mask;
  }

  ~combinations() { delete[] comb_; }

  bool next() {
    // this is to properly hand the case where of Nc0
    if (k_ == 0)
      return (didnull_ = !didnull_);

    int ii = static_cast<int>(k_) - 1;
    while (ii >= 0 && comb_[ii] + k_ + 1 > n_ + ii)
      ii--;
    if (ii < 0)
      return false;
    comb_[ii]++;
    while (static_cast<size_t>(++ii) < k_)
      comb_[ii] = comb_[ii - 1] + 1;
    return true;
  }

  bool nextcomb() { return next(); }

  size_t operator[](size_t i) const { return comb_[i]; }

  size_t size() const { return k_; }

  size_t* begin() { return comb_; }

  const size_t* begin() const { return comb_; }

  size_t* end() { return comb_ + k_; }

  const size_t* end() const { return comb_ + k_; }

 private:
  size_t* comb_;
  size_t n_;
  size_t k_;
  bool didnull_;
};

}  // namespace nit

#endif  // NIT_UTIL_COMBINATIONS_H_
