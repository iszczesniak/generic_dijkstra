#ifndef GENERIC_PERMANENT2_HPP
#define GENERIC_PERMANENT2_HPP

#include <cassert>
#include <set>
#include <tuple>
#include <vector>

// We have to decide how we compare labels when we store and search
// for them in sorted containers.  We compare generic_label's with <
// (that compares first cost then resources) in generic_permanent.
//
// Container generic_permanent2 compares differently: the resources
// first, then the cost.  Here we have to use std::set (and not a
// vector) because resources of an inserted label can be any and they
// do not have to come in the order defined by < for resources.  In
// contrast, generic_permanent stores labels in a vector sorted by
// cost first then by resources (< for generic_label) because labels
// of non-decreasing cost are inserted at the back.

// This functor establishes the required lexicographic ordering (that
// is transitive).  Even though this type is integral to
// generic_permanent2, we cannot define it as its member-type, because
// it is needed in the inheritance list of generic_permanent2.  Also,
// we need to specialize this type for accounting, and it's easier to
// do when it's non-member.
template <typename Label>
struct generic_permanent2_cmp
{
  bool
  operator()(const Label &a, const Label &b) const
  {
    auto s = get_resources(a) <=> get_resources(b);

    // Label a should go before label b, i.e., (a, b) == true, if
    // label a compares lexicographically better: resources are
    // compared first with >, and the cost next with <.
    //
    // We can't use std::tie because it uses <, and we need > and <.
    return s == std::strong_ordering::greater ||
      s == std::strong_ordering::equal &&
      get_weight(a) < get_weight(b);
  }
};

// The container type for storing permanent generic labels.  A key can
// have many labels or none, so we store them in a sorted container.
template <typename Label>
struct generic_permanent2:
  std::vector<std::set<Label, generic_permanent2_cmp<Label>>>
{
  // The label type.
  using label_type = Label;
  using cmp_type = generic_permanent2_cmp<label_type>;

  // The base type.
  using base = std::vector<std::set<label_type, cmp_type>>;
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
      // We can break the loop once we know that Callable()(j, i)
      // holds.  It holds in two cases:
      //
      // * the resources of j and i are equal, but the cost of j is
      //   smaller than the cost of i,
      //
      // * the resources of i cannot include the resources of j.
      //
      // For any label i2 that follows i in the container, relation
      // Callable()(j, i2) holds because Callable is transitive, and
      // labels in the container are sorted according to Callable.
      //
      // This is the order of resources defined by <:
      //
      // *---o
      // *-o
      //  *---o   <- These are the resources of label i.
      //  *--o    <- These are the resources of label j.
      //  *-o     <- These are the resources of label i2.
      //   *----o
      //       *--o
      //
      // As shown above, the resources of i2 cannot include the
      // resources of j because the resources of j can:
      //
      // * include the resources of i2,
      //
      // * overlap with the resources of i2,
      //
      // * precede the resource of i2.
      if (generic_permanent2_cmp<Label>()(j, i))
        break;

      // Is label i better than or equal to label j?
      if (boe(i, j))
        return true;
    }

  return false;
}

#endif // GENERIC_PERMANENT2_HPP
