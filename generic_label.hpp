#ifndef GENERIC_LABEL_HPP
#define GENERIC_LABEL_HPP

#include <compare>

// The label has cost c, and resources r.

template <typename Cost, typename Resources>
struct generic_label
{
  Cost m_c;
  Resources m_r;

  generic_label(Cost c, Resources r):
    m_c(c), m_r(r)
  {
  }

  std::strong_ordering
  operator <=> (const generic_label &) const = default;
};

template <typename Cost, typename Resources>
const auto &
get_cost(const generic_label<Cost, Resources> &l)
{
  return l.m_c;
}

template <typename Cost, typename Resources>
const auto &
get_resource(const generic_label<Cost, Resources> &l)
{
  return l.m_r;
}

// This "better or equal" function.
template <typename Cost, typename Resources>
bool
boe(const generic_label<Cost, Resources> &i,
    const generic_label<Cost, Resources> &j)
{
  return get_cost(i) <= get_cost(j) &&
    includes(get_resources(i), get_resources(j));
}

#endif // GENERIC_LABEL_HPP
