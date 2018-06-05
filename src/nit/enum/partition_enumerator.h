/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef NIT_ENUM_PARTITION_ENUMERATOR_H_
#define NIT_ENUM_PARTITION_ENUMERATOR_H_

#include <cstdint>

#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>

#include <nit/util/combinations.h>

namespace nit {

// see http://cplusplus.com/reference/algorithm/set_difference/
// for original version.
// this version is shifted a bit as we use first1+first2 instead of first1
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator myset_difference(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result) {
  InputIterator1 first = first1;
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *(first + *first2))
      *result++ = *first1++;
    else if (*(first + *first2) < *first1)
      first2++;
    else {
      first1++;
      first2++;
    }
  }
  return copy(first1, last1, result);
}

/**
 * this class enumerates over all partistions of a set of data
 * *given* the size of the partitions.
 */
class PartitionEnumerator2 {
 public:
  /**
   * create a partition enumerator
   *
   * @setsize the size of the index set to enumerate over
   * @partitions the list of partition sizes to use for enumeration
   */
  PartitionEnumerator2(size_t setSize, const std::vector<size_t> partitions)
      : m_setSize(setSize),
        m_parts(partitions),
        m_pcombos(),
        m_subsets(),
        m_masks(partitions.size()) {
    int used = 0;
    for (size_t i = 0; i < m_parts.size(); i++) {
      m_pcombos.emplace_back(m_setSize - used, m_parts[i]);
      m_subsets.emplace_back(m_setSize - used);
      used += m_parts[i];
      setup(static_cast<int>(i));
    }
  }

  /**
   * create a string of all indices
   */
  std::string index_str() const {
    std::string ret;
    for (size_t p = 0; p < numParts(); p++) {
      ret += (p ? " {" : "{");
      for (size_t i = 0; i < partSize(p); i++) {
        auto el = boost::lexical_cast<int>(getIndex(p, i));
        ret += (i ? " " : "") + std::to_string(el);
      }
      ret += "}";
    }
    return ret;
  }

  /**
   * string representation
   */
  std::string str() const {
    std::string ret;
    for (size_t p = 0; p < numParts(); p++) {
      ret += (p ? " {" : "{");
      for (size_t i = 0; i < partSize(p); i++) {
        auto el = boost::lexical_cast<int>(get(p, i));
        ret += (i ? " " : "") + std::to_string(el);
      }
      if (partSize(p) == 0)
        ret += "e";
      ret += "}";
    }
    return ret;
  }

  size_t numParts() const { return m_parts.size(); }

  /**
   * size of a given partition
   */
  size_t partSize(size_t p) const { return m_parts[p]; }

  /**
   * the the contents of specific part
   */
  size_t getIndex(size_t partnum, size_t index) const {
    return m_pcombos[partnum][index];
  }

  size_t get(size_t partnum, size_t index) const {
    return m_subsets[partnum][m_pcombos[partnum][index]];
  }

  uint64_t getMask(size_t partnum) const { return m_masks[partnum]; }

  std::vector<size_t> get(size_t partnum) const {
    std::vector<size_t> ret(m_parts[partnum]);
    for (size_t i = 0; i < m_parts[partnum]; i++)
      ret[i] = m_subsets[partnum][m_pcombos[partnum][i]];
    return ret;
  }

  bool next() { return incr(); }

 private:
  size_t m_setSize;
  std::vector<size_t> m_parts;
  std::vector<combinations> m_pcombos;
  std::vector<std::vector<size_t>> m_subsets;
  mutable std::vector<uint64_t> m_masks;

  bool incr() { return incr(numParts() - 1); }

  void makeMask(size_t partnum) const {
    if (partnum == 0) {
      m_masks[0] = m_pcombos[0].getMask();
    } else {
      const size_t* c = m_pcombos[partnum].begin();
      auto s = m_subsets[partnum].begin();
      uint64_t ret = 0;

      // this loop is very expensive, it accounts for about *half*
      // of the total enumeration time
      for (size_t i = 0; i < m_parts[partnum]; i++)
        ret |= UINT64_C(0x01) << s[*c++];
      m_masks[partnum] = ret;
    }
  }

  // increment to the nth subset
  // this is meant to simulate an odomoeter running when a slot flips,
  // all slots further along "reset".  If a slot runs out of values,
  // then the slot before it flips
  bool incr(int n) {
    // workaround warning in whiel (always true);
    // TODO: verify not buggies!
    // int n = num;
    while (n >= 0) {
      if (m_pcombos[n].next()) {
        makeMask(n);
        while (static_cast<std::size_t>(++n) < m_parts.size())
          setup(n);
        return true;
      }
      if (n == 0) {
        return false;
      }
      n--;
    }
    return false;
  }

  void setup(int n)  // setup the nth subset for enumerating
  {
    if (n >= static_cast<int>(m_parts.size()))
      return;

    // set up the list for this combinator
    if (n - 1 >= 0) {
      myset_difference(m_subsets[n - 1].begin(), m_subsets[n - 1].end(),
                       m_pcombos[n - 1].begin(), m_pcombos[n - 1].end(),
                       m_subsets[n].begin());
    } else if (n == 0) {
      size_t i = 0;
      for (size_t& it : m_subsets[0])
        it = i++;
    }
    m_pcombos[n].reset();
    makeMask(n);
  }
};

}  // namespace nit

#endif  // NIT_ENUM_PARTITION_ENUMERATOR_H_
