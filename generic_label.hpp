#ifndef GENERIC_LABEL_HPP
#define GENERIC_LABEL_HPP

#include "graph_interface.hpp"
#include "props.hpp"

#include <iostream>

// The label has weight c, and resources r.

template <typename Weight, typename Resources>
struct generic_label: weight<Weight>, resources<Resources>
{
  generic_label(Weight w, Resources r):
    weight<Weight>(w), resources<Resources>(r)
  {
  }

  // The default implementation offers a lexicographic comparison.
  constexpr auto operator <=> (const generic_label &) const = default;
};

// This "better or equal" function.
template <typename Weight, typename Resources>
bool
boe(const generic_label<Weight, Resources> &i,
    const generic_label<Weight, Resources> &j)
{
  return get_weight(i) <= get_weight(j) &&
    includes(get_resources(i), get_resources(j));
}

template <template<typename> typename C, typename Label>
bool
boe(const C<Label> &c, const Label &j)
{
  // We could go for the easy implementation: iterate for each label
  // i, and compare it to label j.  But we take advantage of the fact
  // that the elements in the vector are sorted with operator <.  The
  // < operator establishes order between incomparable elements: label
  // i < label i+1.  The stored elements should be incomparable with
  // each other.  We start with the first label as it's the most
  // likely to be better than or equal to j.
  for (const auto &i: c)
    {
      // Stop searching when we reach label i such that j < i, because
      // then label i cannot be better than or equal to j; label j can
      // be better than or incomparable with i.  The same applies to
      // the labels that follow i, because the permanent labels are
      // sorted with <, and < is transitive.  Therefore we can break
      // the loop.
      if (j < i)
        break;

      // Here we know that i <= j, but labels i and j can be
      // incomparable.  Therefore we need to check whether label i
      // better than or equal to label j.
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

// *******************************************************************
// The weight traits

template <typename Weight, typename Resources>
struct weight_traits<generic_label<Weight, Resources>>
{
  using type = Weight;
};

#endif // GENERIC_LABEL_HPP
