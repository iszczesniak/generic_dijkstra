#ifndef LABEL_ROBE_HPP
#define LABEL_ROBE_HPP

#include "generic_label.hpp"
#include "props.hpp"

// A label with a key that is required by the generic_permanent.
template <typename Units>
struct label_robe: generic_label<unsigned, Units>, key<unsigned>
{
  using label_type = generic_label<unsigned, Units>;

  label_robe(const label_type &l, unsigned i):
    label_type(l), key<unsigned>(i)
  {
  }

  // We delegate == and <=> to label_type.  We do not want the default
  // implementations, because the key should not take part.
  constexpr bool operator == (const label_robe &l) const
  {
    return static_cast<const label_type &>(*this)
      == static_cast<const label_type &>(l);
  }

  constexpr auto operator <=> (const label_robe &l) const
  {
    return static_cast<const label_type &>(*this)
      <=> static_cast<const label_type &>(l);
  }
};

#endif // LABEL_ROBE_HPP
