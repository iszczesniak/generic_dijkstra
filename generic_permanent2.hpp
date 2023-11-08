#ifndef GENERIC_PERMANENT2_HPP
#define GENERIC_PERMANENT2_HPP

#include <cassert>
#include <set>
#include <vector>

// This container is experimental.  Needs fixing because it's broken.

// It stores the labels sorted by the resources only.

// This type establishes the order between elements in a set.  We only
// care about the CUs, and not the cost.  In the set we do not store
// labels of equal CUs and different cost.
template <typename Label>
struct cmp
{
  bool
  operator()(const Label &a, const Label &b) const
  {
    return get_resources(a) < get_resources(b);
  }
};

// The container type for storing permanent generic labels.  A vertex
// can have many labels or none, so we store them in a container.
template <typename Label>
struct generic_permanent2:
  std::vector<std::set<Label, cmp<Label>>>
{
  // The label type.
  using label_type = Label;

  // The base type.
  using base = std::vector<std::set<label_type, cmp<label_type>>>;
  // The size type of the base.
  using size_type = typename base::size_type;

  generic_permanent2(size_type count): base(count)
  {
  }

  // Pushes a new label, and returns a reference to it.
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
  // We could go for the easy implementation where we iterate for each
  // label i, and compare it to label j.  But we take advantage of the
  // fact that the elements in the vector are sorted by units first.
  for (const auto &i: P[get_key(j)])
    {
      // Stop searching when we reach a label with a higher CU min,
      // because the CU of that label cannot include the CU of j, nor
      // can the labels that follow.
      if (get_resources(i).min() > get_resources(j).min())
        break;

      // Is label i better than or equal to label j?
      if (i <= j)
        return true;
    }

  return false;
}

#endif // GENERIC_PERMANENT2_HPP
