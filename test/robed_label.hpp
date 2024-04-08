#ifndef ROBED_LABEL_HPP
#define ROBED_LABEL_HPP

#include "generic_label.hpp"
#include "props.hpp"
#include "units.hpp"

using label = generic_label<double, CU>;

// We need this structure, so that we define the key that is required
// by the generic_permanent.
struct robed_label: label, key<unsigned>
{
  using label_type = label;

  robed_label(const label_type &l, unsigned i):
    label_type(l), key<unsigned>(i)
  {
  }

  constexpr auto operator == (const robed_label &l) const
  {
    return label_type::operator==(l);
  }
  
  // We delegate <=> to label_type.
  constexpr auto operator <=> (const robed_label &l) const
  {
    return label_type::operator<=>(l);
  }
};

#endif // ROBED_LABEL_HPP
