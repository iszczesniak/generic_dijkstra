#ifndef GENERIC_LABEL_HPP
#define GENERIC_LABEL_HPP

#include <compare>
#include <iostream>

// The label has weight c, and resources r.

template <typename Weight, typename Resources>
struct generic_label
{
  Weight m_c;
  Resources m_r;

  generic_label(Weight c, Resources r):
    m_c(c), m_r(r)
  {
  }

  // The default implementation offers a lexicographic comparison, and
  // that's what we need.
  constexpr auto
  operator <=> (const generic_label &) const = default;
};

template <typename Weight, typename Resources>
const auto &
get_weight(const generic_label<Weight, Resources> &l)
{
  return l.m_c;
}

template <typename Weight, typename Resources>
const auto &
get_resources(const generic_label<Weight, Resources> &l)
{
  return l.m_r;
}

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

// *******************************************************************
// The weight traits

template<typename>
struct weight_traits;

template <typename Weight, typename Resources>
struct weight_traits<generic_label<Weight, Resources>>
{
  using type = Weight;
};

template <typename T>
using Weight = typename weight_traits<T>::type;

#endif // GENERIC_LABEL_HPP
