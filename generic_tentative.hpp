#ifndef GENERIC_TENTATIVE_HPP
#define GENERIC_TENTATIVE_HPP

#include <cassert>
#include <set>
#include <tuple>
#include <vector>

// The container type for storing the generic tentative labels.  We
// keep the labels separate for every key because functions
// has_better_or_equal and purge_worse should go through labels for a
// specific key only, not through all labels for all keys.
//
// For a given key, the labels are stored in a set sorted with <.
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

  // The functor structure for comparing the labels in the queue.
  struct cmp
  {
    base_type &m_r;

    cmp(base_type &r): m_r(r)
    {
    }

    bool operator()(const size_type &a, const size_type &b) const
    {
      // We compare the first elements of the sets, because they are
      // the best the sets offer, i.e. the elements in the sets are
      // sorted with <.  We need to compare also a and b, because we
      // need to differentiate between the elements of the queue that
      // compare equivalent, so that we can add and remove specific
      // keys.

      return std::tie(*m_r.operator[](a).begin(), a) <
        std::tie(*m_r.operator[](b).begin(), b);
    }
  };

  // The set of keys that serves as the priority queue.
  std::set<size_type, cmp> m_pq;

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
      {
        assert(m_pq.contains(key));
        m_pq.erase(key);
      }

    // Remove the labels that are worse than or equal to l.  We want
    // to remove those labels now, before we insert l, because we're
    // using the boe (better or equal) function for comparison.
    purge_worse_or_equal(vd, l);

    // Insert the new label to the set.
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
      {
        auto [i, status] = m_pq.insert(key);
        assert(status);
      }

    // The key must be in the queue.
    assert(m_pq.contains(key));

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
      {
        auto [i, status] = m_pq.insert(key);
        assert(status);
      }

    // There are no labels in vd or the key is in m_pq.  If there are
    // no labels, then we cannot make sure the key is not in the
    // queue, because functor cmp would accces a label in an empty vd.
    assert(vd.empty() || m_pq.contains(key));

    return std::move(nh.value());
  }

private:
  // Purge from vd those labels i that are worse than or equal to j,
  // i.e., those for which boe(j, i) is true.
  void
  purge_worse_or_equal(vd_type &vd, const label_type &j)
  {
    // Since labels (for a given key) are sorted with <, we:
    //
    // * iterate in the reverse order because the worse labels are
    //   most likely at the end,
    //
    // * break the loop once we discover that i < j because then i can
    //   only be better than or incomparable with j (so i cannot be
    //   worse), and the same applies to the labels that come before i
    //   because < is transitive.
    for(auto r = vd.rbegin(); r != vd.rend();)
      {
        const auto &i = *r;

        if (i < j)
          break;

        // Check whether j is better than or equal to i.
        if (boe(j, i))
          {
            // We want to remove label i, and we're going to use
            // iterator r.  We can safely remove the element pointed
            // to by r, because the base iterator points to the
            // element left to r.  This erasure does not invalidate
            // the base iterator of r.  Note that we do not increment
            // r, because after the erasure, r will already point to
            // the next element (on the left).
            vd.erase(--(r.base()));
          }
        else
          ++r;
      }
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

#endif // GENERIC_TENTATIVE_HPP
