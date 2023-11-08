#ifndef GENERIC_IOSTREAM_HPP
#define GENERIC_IOSTREAM_HPP

#include "generic_label.hpp"

#include <iostream>

template <typename Weight, typename Resources>
std::ostream &
operator<<(std::ostream &out,
           const generic_label<Weight, Resources> &l)
{
  out << "label(weight = " << get_weight(l)
      << ", resources = " << get_resources(l) << ")";

    return out;
}

#endif // GENERIC_IOSTREAM_HPP
