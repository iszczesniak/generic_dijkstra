#ifndef GENERIC_PERMANENT2_HPP
#define GENERIC_PERMANENT2_HPP

#include <cassert>
#include <set>
#include <tuple>
#include <vector>

// We have to decide how we compare labels when we store (in sorted
// containers) and search for them.  Operator < for generic_label
// first compares the cost, then the resources.  Container
// generic_permanent uses <.
//
// Container generic_permanent2 compares differently: the resources
// first, then the cost.  Here we have to use std::set because
// resources of an inserted label can be any and they do not have to
// come one after another, i.e., in the order defined by < for
// resources.  In contrast, in generic_permanent labels are stored in
// a vector that is sorted by cost first (< for generic_label) because
// labels of non-decreasing cost are inserted at the back.
struct cmp
{
  template <typename Label>
  bool
  operator()(const Label &a, const Label &b) const
  {
    return std::tie(get_resources(a), get_weight(a)) <
      std::tie(get_resources(b), get_weight(b));
  }
};

// The container type for storing permanent generic labels.  A key can
// have many labels or none, so we store them in a sorted container.
template <typename Label>
struct generic_permanent2: std::vector<std::set<Label, cmp>>
{
  // The label type.
  using label_type = Label;

  // The base type.
  using base = std::vector<std::set<label_type, cmp>>;
  // The size type of the base.
  using size_type = typename base::size_type;

  generic_permanent2(size_type count): base(count)
  {
  }

  // Pushes a new label, and returns a reference to it.  We return a
  // const reference because a label stays there for good unchanged.
  template <typename T>
  const label_type &
  push(T &&l)
  {
    // The key of the label.
    const auto &ti = get_key(l);
    // The vertex data.
    auto &vd = base::operator[](ti);
    // Just insert.
    auto [i, s] = vd.insert(std::forward<T>(l));
    assert(s);
    // Return reference to the inserted element.
    return *i;
  }
};

/**
 * Is there in P a label that is better than or equal to label j?
 */
template <typename Label>
bool
has_better_or_equal(const generic_permanent2<Label> &P,
                    const Label &j)
{
  // We have to iterate from the beginning and cannot use lower_bound
  // or upper_bound because the resources of the first label can
  // include the resources of j.
  for (const auto &i: P[get_key(j)])
    {
      // We can break the loop once we know that the resources of i
      // cannot include the resources of j, so i cannot be better or
      // equal to j.
      if (cmp()(j, i))
        break;

      // Is label i better than or equal to label j?
      if (boe(i, j))
        return true;
    }

  return false;
}

#endif // GENERIC_PERMANENT2_HPP
