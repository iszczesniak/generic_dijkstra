#ifndef LABEL_HPP
#define LABEL_HPP

#include "generic_label.hpp"
#include "units.hpp"

using label = generic_label<double, CU>;

struct label_robe: generic_label<double, CU>, index<unsigned>
{
  using label_type = generic_label<double, CU>;

  label_robe(const label_type &l, unsigned i):
    label_type(l), index<unsigned>(i)
  {
  }

  using label_type::operator==;
  using label_type::operator<=>;
};

#endif // LABEL_HPP
