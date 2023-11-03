#ifndef LABEL_HPP
#define LABEL_HPP

#include "generic_label.hpp"
#include "units.hpp"

using label = generic_label<double, CU>;

struct robed_label: label, index<unsigned>
{
  using label_type = label;

  robed_label(const label_type &l, unsigned i):
    label_type(l), index<unsigned>(i)
  {
  }

  // We delegate <=> to label_type.
  constexpr auto operator <=> (const robed_label &l) const
  {
    return label_type::operator<=>(l);
  }
};

#endif // LABEL_HPP
