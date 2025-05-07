#ifndef GENERIC_LABEL_HPP
#define GENERIC_LABEL_HPP

#include "props.hpp"

#include <iostream>

// The label has weight c, and resources r.

template <typename Weight, typename Resources>
struct generic_label: weight<Weight>, resources<Resources>
{
  generic_label(Weight w, const Resources &r):
    weight<Weight>(w), resources<Resources>(r)
  {
  }

  bool operator == (const generic_label &) const = default;

  // If i < j, then i is better than j.
  auto
  operator <=> (const generic_label &j) const
  {
    // Label i should go before (be less than) j if it has smaller
    // cost.
    if (get_weight(*this) < get_weight(j))
      return std::strong_ordering::less;
    if (get_weight(*this) > get_weight(j))
      return std::strong_ordering::greater;

    // Now we know the costs are equal, so the resources have to
    // decide.  Label i should go before (be less than) j as per > for
    // resources.
    if (get_resources(*this) > get_resources(j))
      return std::strong_ordering::less;
    if (get_resources(*this) < get_resources(j))
      return std::strong_ordering::greater;

    return std::strong_ordering::equal;
  }
};

// The "better or equal" function.
template <typename Weight, typename Resources>
bool
boe(const generic_label<Weight, Resources> &i,
    const generic_label<Weight, Resources> &j)
{
  return get_weight(i) <= get_weight(j) &&
    includes(get_resources(i), get_resources(j));
}

// We assume that labels in C are:
//
// * sorted with <, i.e., for any iterators i1 < i2 in C, *i1 < *i2
//   holds, and
//
// * boe-incomparable, i.e., there are no labels l1, l2 in C such that
//   boe(l1, l2) holds.
template <template<typename...> typename C, typename Label>
bool
boe(const C<Label> &c, const Label &j)
{
  // We don't have to iterate through all labels since they are sorted
  // with <.  We stop when further search is futile.
  for (const auto &i: c)
    {
      // Stop searching when we reach label i such that j < i because
      // then label i cannot be better than or equal to j; label j can
      // be better than or incomparable with i.  The same applies to
      // the labels that follow i, because the permanent labels are
      // sorted with <, and < is transitive.  Therefore we can break.
      if (j < i)
        break;

      // Here we know that i <= j, but labels i and j can be
      // boe-incomparable.  Therefore we need to check whether label i
      // is better than or equal to label j.
      if (boe(i, j))
        return true;
    }

  return false;
}

template <typename Weight, typename Resources>
std::ostream &
operator<<(std::ostream &out,
           const generic_label<Weight, Resources> &l)
{
  out << "label(weight = " << get_weight(l)
      << ", resources = " << get_resources(l) << ")";

    return out;
}

#endif // GENERIC_LABEL_HPP
