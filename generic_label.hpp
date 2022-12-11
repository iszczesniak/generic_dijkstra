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

  // The default implementation offers a lexicographic comparison, and
  // that's what we need.
  constexpr auto
  operator <=> (const generic_label &) const = default;
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
