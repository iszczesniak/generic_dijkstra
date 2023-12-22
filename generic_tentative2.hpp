#ifndef GENERIC_TENTATIVE_HPP
#define GENERIC_TENTATIVE_HPP

#include <cassert>
#include <set>
#include <vector>

// The container type for storing the generic tentative labels.  We
// keep the labels separate for every key because functions
// has_better_or_equal and purge_worse go through labels for a
// specific key only.
//
// For a given key, the labels stored are sorted with <.
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

  // The functor structure for comparing the elements of the queue.
  struct cmp
  {
    base_type &m_r;

    cmp(base_type &r): m_r(r)
    {
    }

    bool operator()(const size_type &a, const size_type &b) const
    {
      // We compare only the first elements of the sets, because they
      // are the best the sets offer, i.e. the elements in the sets
      // are sorted with <.
      return *m_r.operator[](a).begin() < *m_r.operator[](b).begin();
    }
  };

  // The set of keys that serves as the priority queue.  We need the
  // multiset, because there can be keys (of different values) that
  // compare equivalent with the < operator (i.e., < doesn't hold
  // between them): the keys refer to equal labels for different
  // vertexes.
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
    // The key of the label.
    auto key = get_key(l);
    // The set of labels for the key.
    auto &vd = base_type::operator[](key);

    // If inserting label l would push back the existing label to
    // which the key in the priority queue is referring, we have to
    // remove the key from the queue.  Otherwise we would corrupt the
    // queue.
    if (!vd.empty() && l < *vd.begin())
      m_pq.erase(key);

    auto [i, s] = vd.insert(std::forward<T>(l));
    // Make sure the insertion was successful.
    assert(s);

    // Insert the key to the priority queue only if the label ended up
    // at the beginning of the set, which can happen for one of two
    // reasons:
    //
    // * it's the only label in the set, and so the key was not
    //   inserted before,
    //
    // * it's not the only label in the set, but we knew that it would
    //   be first, and so we removed the key before.
    //
    // If the label is inserted in the set after the first element,
    // then there is no need to add the key into the queue, because
    // there already is one for the first label in the set.
    if (i == vd.begin())
      m_pq.insert(key);
    
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
    // Get the key from the queue.
    size_type key = *m_pq.begin();
    m_pq.erase(m_pq.begin());
    // Get the set for the key.
    auto &vd = base_type::operator[](key);
    assert(!vd.empty());
    // Get the first element.
    auto nh = vd.extract(vd.begin());
    // Insert the key again if the set is not empty.
    if (!vd.empty())
      m_pq.insert(key);

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
  return boe(T[get_key(j)], j);
}

/**
 * Purge those that are worse than j.
 */
template <typename Label>
void
purge_worse(generic_tentative<Label> &T, const Label &j)
{
  auto key = get_key(j);
  auto &Tt = T[key];
  bool empty_before = Tt.empty();

  // Since labels (for a given key) are sorted with <, we:
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

  // We remove the key from the priority queue only when there are no
  // labels for this key while previously there where.
  if (!empty_before && Tt.empty())
    T.m_pq.erase(key);
}

#endif // GENERIC_TENTATIVE_HPP