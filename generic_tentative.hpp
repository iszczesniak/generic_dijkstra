#ifndef GENERIC_TENTATIVE_HPP
#define GENERIC_TENTATIVE_HPP

#include "graph_interface.hpp"

#include <cassert>
#include <set>
#include <vector>

// The container type for storing the generic tentative labels.  We
// keep the labels separate for every index because functions
// has_better_or_equal and purge_worse go through labels for a
// specific index only.
//
// For a given index, the labels stored are sorted with <.
template <typename Label>
struct generic_tentative: std::vector<std::set<Label>>
{
  // The label type.
  using label_type = Label;
  // The type of data a vertex has.
  using vd_type = std::set<label_type>;
  // The type of the vector of vertex data.
  using base_type = std::vector<vd_type>;
  // The size type of the base type.
  using size_type = typename base_type::size_type;

  // The functor structure for comparing the elements of the queue.  I
  // tried to rewrite this as a lambda, but it failed to compile.
  struct cmp
  {
    base_type &m_r;

    cmp(base_type &r): m_r(r)
    {
    }

    bool operator()(const size_type &a, const size_type &b) const
    {
      return *m_r.operator[](a) < *m_r.operator[](b);
    }
  };

  // The set of indexes that serves as the priority queue.  We need
  // the multiset, because there can be labels that compare equivalent
  // with the < operator (i.e., < doesn't hold between them): equal
  // labels for different vertexes.
  std::multiset<size_type, cmp> m_pq;

  // The constructor builds a vector of data for each vertex.
  generic_tentative(size_type count): base_type(count), m_pq(*this)
  {
  }

  // This function pushes a new label, and returns a reference to the
  // label in the container.
  template<typename T>
  const auto &
  push(T &&l)
  {
    // The index of the target vertex.
    auto index = get_index(l);
    auto &vd = base_type::operator[](index);
    auto [i, s] = vd.insert(std::forward<T>(l));
    // Make sure the insertion was successful.
    assert(s);

    if (i == vd.begin())
      {
        auto c = get_weight(l);
        // There already can be an element in the queue for tk.
        auto &o = m_v2c[ti];
        if (o)
          // Erase the former element from the queue.
          m_pq.erase({*o, ti});
        m_pq.insert({c, ti});
        o = c;
      }

    return *i;
  }

  bool
  empty() const
  {
    return m_pq.empty();
  }

  // Here we return a label by value.
  auto
  pop()
  {
    assert(!m_pq.empty());
    const auto [c, ti] = *m_pq.begin();
    m_pq.erase(m_pq.begin());
    auto &vd = base_type::operator[](ti);
    assert(!vd.empty());
    auto nh = vd.extract(vd.begin());
    assert(get_weight(nh.value()) == c);
    auto &o = m_v2c[ti];
    // If there is other label for ti, put it into the queue.
    if (!vd.empty())
      {
        const auto &nc = get_weight(*vd.begin());
        assert(get_index(*vd.begin()) == ti);
        m_pq.insert({nc, ti});
        o = nc;
      }
    else
      o.reset();

    return std::move(nh.value());
  }
};

/**
 * Is there in T a label that is better than or equal to label j?
 */
template <typename Label>
bool
has_better_or_equal(const generic_tentative<Label> &T, const Label &j)
{
  return boe(T[get_index(j)], j);
}

/**
 * Purge those that are worse than j.
 */
template <typename Label>
void
purge_worse(generic_tentative<Label> &T, const Label &j)
{
  auto index = get_index(j);
  auto &Tt = T[index];
  bool empty_before = Tt.empty();

  // Since labels (for a given index) are sorted with <, we:
  //
  // * iterate in the reverse order because the worse labels are most
  //   likely at the end,
  //
  // * break the loop once we discover that i < j because then i can
  //   only be better than or incomparable with j (so i cannot be
  //   worse), and the same applies to the labels that come before i
  //   because < is transitive.
  
  for(auto r = Tt.rbegin(); r != Tt.rend();)
    {
      const auto &i = *r;

      if (i < j)
        break;

      // Check whether j is better than or equal to i.
      if (boe(j, i))
        // We want to remove label i, and we're going to use iterator
        // r.  We can safely remove the element pointed to by r,
        // because the base iterator points to the element left to r.
        // This erasure does not invalidate the base iterator of r.
        // Note that we do not increment r, because after the erasure,
        // r will already point to the next element (on the left).
        Tt.erase(--(r.base()));
      else
        ++r;
    }

  // We remove the index from the priority queue only when there are
  // no labels for this index while previously there where.
  if (!empty_before && Tt.empty())
    m_pq.erase(index);
}

#endif // GENERIC_TENTATIVE_HPP
